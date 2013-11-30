#pragma once


namespace miki
{

struct loadsound_exception : std::exception{};

namespace detail
{

struct sound_impl
{
	explicit sound_impl( std::string const& fname )
		:handle( LoadSoundMem(fname.c_str()) )
	{
		BOOST_ASSERT( handle != -1);
	}

	~sound_impl()
	{
		DeleteSoundMem( handle );
	}

	int handle;
};

} // namespace detail

typedef
	boost::flyweights::flyweight
		< boost::flyweights::key_value< std::string
									  , detail::sound_impl > >
		//, boost::flyweights::hashed_factory< detail::imagekey_hash > >
	sound
	;


inline int to_play( miki::sound const& snd )
{
	return snd.get().handle;
}
inline sound make_sound( std::string const& fname ){
	return sound(fname);
}


} // namespace miki