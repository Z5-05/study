#include <atomic>
#include <functional>
#include <memory>
#include <thread>
#include <stdexcept>
#include <iostream>


unsigned int const max_hazard_pointers = 100;
struct hazard_pointer_t {
	std::atomic<std::thread::id> id;
	std::atomic<void *> pointer;
};

hazard_pointer_t hazard_pointers[ max_hazard_pointers ];

class hp_owner_t {
private:
	hazard_pointer_t * m_hp;
public:
	hp_owner_t();
	~hp_owner_t();
	auto get_pointer() -> std::atomic<void *> &;
};

hp_owner_t::hp_owner_t() :
	m_hp{ nullptr }
{
	for( unsigned int i = 0; i < max_hazard_pointers; ++i ) {
		std::thread::id old_id;
		std::cout << old_id;
		if( hazard_pointers[ i ].id.compare_exchange_strong( old_id, std::this_thread::get_id() ) ) {
			m_hp = &hazard_pointers[ i ];
			break;
		}
	}
	if( !m_hp ) {
		throw std::runtime_error{ "No hazard pointer available" };
	}
}

hp_owner_t::~hp_owner_t()
{
	m_hp->pointer.store( nullptr );
	m_hp->id.store( std::thread::id{} );
}

auto hp_owner_t::get_pointer() -> std::atomic<void *> &
{
	return m_hp->pointer;
}

auto get_hazard_pointer_for_current_thread() -> std::atomic<void *> &
{
	thread_local static hp_owner_t hazard;
	return hazard.get_pointer();
}

bool outstanding_hazard_pointers_for( void * ptr )
{
	for( unsigned int i = 0; i < max_hazard_pointers; ++i ) {
		if( hazard_pointers[ i ].pointer.load() == ptr ) {
			return true;
		}
	}

	return false;
}

template< typename T >
void do_delete( void * ptr )
{
	delete static_cast<T *>( ptr );
}

struct data_to_reclaim_t {
	void * data;
	std::function<void( void * )> deleter;
	data_to_reclaim_t * next;

	template< typename T >
	data_to_reclaim_t( T * ptr );
	~data_to_reclaim_t();
};


template< typename T >
data_to_reclaim_t::data_to_reclaim_t( T * ptr ) :
	data{ ptr },
	deleter{ &do_delete<T> },
	next{ nullptr }
{
}

data_to_reclaim_t::~data_to_reclaim_t()
{
	deleter( data );
}

std::atomic<data_to_reclaim_t *> nodes_to_reclaim{ nullptr };

void add_to_reclaim_list( data_to_reclaim_t * node )
{
	node->next = nodes_to_reclaim.load();
	while( !nodes_to_reclaim.compare_exchange_weak( node->next, node ) ) {
		continue;
	}
}

template< typename T >
void reclaim_later( T * data )
{
	add_to_reclaim_list( new data_to_reclaim_t{ data } );
}

void delete_nodes_with_no_hazard_pointers()
{
	data_to_reclaim_t * current = nodes_to_reclaim.exchange( nullptr );
	while( current ) {
		data_to_reclaim_t * const next = current->next;
		if( !outstanding_hazard_pointers_for( current->data ) ) {
			delete current;
		}
		else {
			add_to_reclaim_list( current );
		}
		
		current = next;
	}
}
