#pragma once

namespace miki
{

template<class Char_T>
struct basic_str_wrap{

   basic_str_wrap( Char_T const* str )
       :fst(str)
	   ,lst( strLast(str) )
	   ,cstr(str)
   {}

   basic_str_wrap( std::basic_string<Char_T> const& str )
       :fst( &*str.cbegin() )
	   ,lst( &*str.cend() )
	   ,cstr( str.c_str() )
    {}

    Char_T const* begin() const{ return fst;}
    Char_T const* cbegin()const{ return fst;}
    Char_T const* end()   const{ return lst;}
    Char_T const* cend()  const{ return lst;}
    Char_T const* c_str() const{ return fst;}

	operator char const* () const{ return cstr; }

private:
	static Char_T const* strLast( Char_T const* str ){
		while ( str != '\0' ) ++str;
		return str;
	}
    Char_T const* fst;
	Char_T const* lst;
	Char_T const* cstr;
};

typedef basic_str_wrap<char> str_wrap;

template<class T>
inline
T limit( T const& min, T const& max, T const& val ){
	return val > max ? max
		             : val < min ? min
					             : val;
}

template<class T>
inline
void lim( T const& min, T const& max, T& val ){
	val = limit( min, max, val );
}

template<class To,class From>
inline To downcast( From* x ){
	return
#ifndef NDEBUG
		[]( From* x ) -> To {
			To t = dynamic_cast<To>( x );
			BOOST_ASSERT( t != nullptr );
			return t;
		}( x );
#else
		static_cast<To>( x );
#endif
}

template<class Iter, class Pred>
inline auto
optional_find_if( Iter const begin, Iter const end, Pred const p )
	-> boost::optional<decltype(*begin)>
{
	auto const fit = std::find_if( begin, end, p );
	return boost::make_optional( fit != end, *fit );
}

template<class Iter, class Value>
inline auto
optional_find( Iter const begin, Iter const end, Value const& v )
	-> boost::optional<decltype(*begin)>
{
	return optional_find_if( begin, end, [&]( std::iterator_traits<Iter>::value_type const& a ){ return a == v; } );
}


template<class Rng, class Pred>
inline auto
optional_find_if( Rng const& rng, Pred const p )
	-> boost::optional< decltype( *boost::begin(rng) ) >
{
	return optional_find( boost::begin( rng ), boost::end( rng ), p );
}

template<class Rng, class Value>
inline auto
optional_find( Rng const& rng, Value const& v )
	-> boost::optional< decltype( *boost::begin(rng) ) >
{
	return optional_find( boost::begin( rng ), boost::end( rng ), v );
}

// is_sorted は標準にあるものの、VSの実装は効率が悪い
// しかしこれは実装が間違ってるかもしれない。怖い。
template<class Iter, class Pred>
inline bool is_sorted( Iter first, Iter const last, Pred const pred ){
	if(first != last){
		for( Iter next = first;; ++first){
			if( ++next == last ) break;
			if( pred( *next, *first ) ) return false;
		}
	}
	return true;
}

// >> << & | ^ の函数オブジェクト
#define HOGE( sname, op ) \
template<class T> \
struct sname{ \
	T operator()( T const& lhs, T const& rhs ) const{ return lhs op rhs; } \
};

	HOGE( bitwise_lshift, << )
	HOGE( bitwise_rshift, >> )
	HOGE( bitwise_or, | )
	HOGE( bitwise_and, & )
	HOGE( bitwise_xor, ^ )

#undef HOGE

struct undefined_t{
	template<class T>
	operator T&() const{
		throw std::exception("undefined must not be used");
	}
};

static const undefined_t undefined;

} // namespace miki

#define RNG(r) r.begin(), r.end()
#define CRNG(r) r.cbegin(), r.cend()

#define MIKI_FOREACH( VAR, RNG )						\
	{													\
		auto&      rng___      = (RNG);                 \
		auto const it___end___ = boost::end( rng___ );	\
		for ( auto it___ = boost::begin( rng___ ); it___ != it___end___; ++it___ ){\
			VAR = *it___ ; \
				{

#define MIKI_FOREACH_END }}}

#ifndef NDEBUG
#define MIKI_DEFAULT \
	default: assert( false && " default " );
#else
#define MIKI_DEFAULT
#endif

#ifndef NDEBUG
#define MIKI_CHECK
#endif

#ifdef MIKI_CHECK
#define M_ASSERT( exp, what ) \
	if ( !(exp) ) \
	{ \
	  MessageBox( NULL, (what), "ERROR", MB_OK ); \
	  abort(); \
	} 
#else
#define M_ASSERT( exp, what )
#endif

#define M_ASSERT2( exp ) M_ASSERT( exp, #exp )