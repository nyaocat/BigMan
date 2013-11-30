#pragma once
#include <DxLib.h>

#include <fstream>
#include <sstream>

#include <array>
#include <exception>
#include <string>
#include <cctype>

#include <vector>
#include <memory>
#include <bitset>
#include <unordered_map>

#include <algorithm>
#include <functional>
#include <numeric>

#include <boost/ref.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#include <boost/assign.hpp>
#include <boost/scoped_ptr.hpp>

#include <boost/progress.hpp>
#include <boost/noncopyable.hpp>
#include <boost/assert.hpp>
#include <boost/checked_delete.hpp>
#include <boost/operators.hpp>
#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include <boost/utility/in_place_factory.hpp>

#include <boost/range.hpp>
#include <boost/range/irange.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <boost/range/numeric.hpp>

#include <boost/flyweight.hpp>
#include <boost/flyweight/key_value.hpp>
#include <boost/functional/hash.hpp>

#include <boost/pool/pool.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <boost/pool/object_pool.hpp>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_expression.hpp>
#include <boost/numeric/ublas/matrix.hpp>

typedef boost::numeric::ublas::vector<double> dvector;
#include "make_unique_nvt.hpp"
#include "image.h"
#include "sound.h"
#include "utils.hpp"

#ifndef NDEBUG
#define DEBUG_OUT(msg) std::cout << msg << std::endl;
#else
#define DEBUG_OUT(msg)  
#endif 

inline bool FileRead_exist( char const* fname ){
	int const fhnd = FileRead_open( fname );
	FileRead_close( fhnd );
	return fhnd != 0;
}

#undef min
#undef max