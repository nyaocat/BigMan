#pragma once


namespace miki
{

struct loadgraph_exception : std::exception{};

namespace detail
{

struct image_key
{
	std::string name;
	int allnum,xnum,ynum,xsize,ysize;

	friend bool operator==( image_key const& lhs, image_key const& rhs )
	{
		return ( lhs.ysize  == rhs.ysize  )
			&& ( lhs.xsize  == rhs.xsize  )
			&& ( lhs.allnum == rhs.allnum )
			&& ( lhs.xnum   == rhs.xnum   )
			&& ( lhs.ynum   == rhs.ynum   )
			&& ( lhs.name   == rhs.name   );
	}
};

struct imagekey_hash{
	size_t operator()( image_key const& k )const
	{
		size_t h = 0;
		boost::hash_combine( h, k.name );
		boost::hash_combine( h, k.xnum );
		boost::hash_combine( h, k.ynum );
		boost::hash_combine( h, k.xsize );
		boost::hash_combine( h, k.ysize );
		boost::hash_combine( h, k.allnum );
		return h;
	}
};

struct image_impl
{
	explicit image_impl(image_key const& imgkey)
		:gs( imgkey.allnum )
	{
		BOOST_ASSERT( ( imgkey.allnum == 1 && imgkey.xnum == 1 && imgkey.ynum == 1 && imgkey.xsize == 1 && imgkey.ysize == 1 )
				   || ( imgkey.allnum > 1 ) );

		if ( imgkey.allnum == 1 ){
			gs.front() = LoadGraph( imgkey.name.c_str() );
			BOOST_ASSERT( gs.front() != -1 );
		}
		else{
#ifndef NDEBUG
			int loadret =
#endif
			LoadDivGraph( imgkey.name.c_str()
						, imgkey.allnum, imgkey.xnum, imgkey.ynum, imgkey.xsize, imgkey.ysize
						, gs.data() );
#ifndef NDEBUG
			BOOST_ASSERT(loadret != -1);
#endif
		}
	}

	~image_impl()
	{
		boost::for_each( gs,[]( int const h ){ DeleteGraph( h ); } );
	}

	std::vector<int> gs;
};

} // namespace detail

typedef
	boost::flyweights::flyweight
		< boost::flyweights::key_value< detail::image_key
									  , detail::image_impl >
		, boost::flyweights::hashed_factory< detail::imagekey_hash > >
	image
	;

inline image make_image( std::string const& imgfile
		        , int   allnum
				, int   xdiv
				, int   ydiv
				, int   xsize
				, int   ysize                       )
{
	detail::image_key tmp = { imgfile,allnum,xdiv,ydiv,xsize,ysize };
	return image( tmp );
}

inline image make_image( std::string const& imgfile )
{
	return make_image( imgfile, 1, 1, 1, 1, 1 );
}

inline int to_draw( miki::image const& img, size_t const i = 0U )
{
	BOOST_ASSERT( img.get().gs.size() > i );
	return img.get().gs[i];
}


} // namespace miki