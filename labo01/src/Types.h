#pragma once

/**
 * @file Types.h
 *
 * @brief Types utilisés pour définir des matrices.
 *
 */

namespace gti320 
{

    enum SizeType
    {
        Dynamic = -1
    };

    enum StorageType
    {
        ColumnStorage = 0,
        RowStorage = 1
    };

    template<typename _ScalarType>
    struct TripletType
    {
        _ScalarType val;
        unsigned int i, j;
    };

}
