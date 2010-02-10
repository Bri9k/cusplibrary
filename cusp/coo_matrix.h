/*
 *  Copyright 2008-2009 NVIDIA Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*! \file coo_matrix.h
 *  \brief Coordinate matrix format.
 */

#pragma once

#include <cusp/detail/config.h>

#include <cusp/array1d.h>
#include <cusp/detail/matrix_base.h>

namespace cusp
{

/*! \addtogroup container_classes Container Classes 
 *  \addtogroup sparse_matrix_formats Sparse Matrices
 *  \ingroup container_classes
 *  \{
 */
    template<typename IndexType, class MemorySpace>
    class coo_pattern : public detail::matrix_base<IndexType>
    {
        public:
        typedef IndexType index_type;

        typedef MemorySpace memory_space;
        typedef typename cusp::choose_memory_allocator<IndexType, MemorySpace>::type index_allocator_type;
        typedef typename cusp::coo_pattern<IndexType, MemorySpace> pattern_type;
        
        template<typename MemorySpace2>
        struct rebind { typedef coo_pattern<IndexType, MemorySpace2> type; };

        cusp::array1d<IndexType, index_allocator_type> row_indices;
        cusp::array1d<IndexType, index_allocator_type> column_indices;

        coo_pattern();

        coo_pattern(IndexType num_rows, IndexType num_cols, IndexType num_entries);

        template <typename IndexType2, typename MemorySpace2>
        coo_pattern(const coo_pattern<IndexType2,MemorySpace2>& pattern);
        
        void resize(IndexType num_rows, IndexType num_cols, IndexType num_entries);
        
        void swap(coo_pattern& pattern);
    }; // class coo_pattern


/*! \p coo_matrix : Coordinate matrix format
 *
 * \tparam IndexType Type used for matrix indices (e.g. \c int).
 * \tparam ValueType Type used for matrix values (e.g. \c float).
 * \tparam MemorySpace Either a memory space such as \c cusp::host_memory or 
 *         \c cusp::device_memory or a specific memory allocator type such as
 *         \c thrust::device_malloc_allocator<T>.
 *
 * \note The matrix entries must be sorted by row and column index.
 * \note The matrix should not contain duplicate entries.
 *
 *  The following code snippet demonstrates how to create a 4-by-3
 *  \p coo_matrix on the host with 6 nonzeros and then copies the
 *  matrix to the device.
 *
 *  \code
 *  #include <cusp/coo_matrix.h>
 *  ...
 *
 *  // allocate storage for (4,3) matrix with 6 nonzeros
 *  cusp::coo_matrix<int,float,cusp::host_memory> A(4,3,6);
 *
 *  // initialize matrix entries on host
 *  A.row_indices[0] = 0; A.columns_indices[0] = 0; A.values[0] = 10;
 *  A.row_indices[1] = 0; A.columns_indices[1] = 2; A.values[1] = 20;
 *  A.row_indices[2] = 2; A.columns_indices[2] = 2; A.values[2] = 30;
 *  A.row_indices[3] = 3; A.columns_indices[3] = 0; A.values[3] = 40;
 *  A.row_indices[4] = 3; A.columns_indices[4] = 1; A.values[4] = 50;
 *  A.row_indices[5] = 3; A.columns_indices[5] = 2; A.values[5] = 60;
 *
 *  // A now represents the following matrix
 *  //    [10  0 20]
 *  //    [ 0  0  0]
 *  //    [ 0  0 30]
 *  //    [40 50 60]
 *
 *  // copy to the device
 *  cusp::coo_matrix<int,float,cusp::device_memory> B = A;
 *  \endcode
 *
 */
    template <typename IndexType, typename ValueType, class MemorySpace>
    class coo_matrix : public coo_pattern<IndexType, MemorySpace>
    {
        public:
        typedef ValueType value_type;

        typedef typename cusp::choose_memory_allocator<ValueType, MemorySpace>::type value_allocator_type;
        typedef typename cusp::coo_matrix<IndexType, ValueType, MemorySpace> matrix_type;
        
        template<typename MemorySpace2>
        struct rebind { typedef coo_matrix<IndexType, ValueType, MemorySpace2> type; };
   
        /*! Storage for the nonzero entries of the COO data structure.
         */
        cusp::array1d<ValueType, value_allocator_type> values;
    
        /*! Construct an empty \p coo_matrix.
         */
        coo_matrix();
    
        /*! Construct a \p coo_matrix with a specific shape and number of nonzero entries.
         *
         *  \param num_rows Number of rows.
         *  \param num_cols Number of columns.
         *  \param num_entries Number of nonzero matrix entries.
         */
        coo_matrix(IndexType num_rows, IndexType num_cols, IndexType num_entries);
    
        /*! Construct a \p coo_matrix from another \p coo_matrix.
         *
         *  \param matrix Another \p coo_matrix.
         */
        template <typename IndexType2, typename ValueType2, typename MemorySpace2>
        coo_matrix(const coo_matrix<IndexType2, ValueType2, MemorySpace2>& matrix);
        
        /*! Construct a \p coo_matrix from another matrix format.
         *
         *  \param matrix Another sparse or dense matrix.
         */
        template <typename MatrixType>
        coo_matrix(const MatrixType& matrix);
   
        void resize(IndexType num_rows, IndexType num_cols, IndexType num_entries);

        /*! Swap the contents of two \p coo_matrix objects.
         *
         *  \param matrix Another \p coo_matrix with the same IndexType and ValueType.
         */
        void swap(coo_matrix& matrix);

        /*! Assignment from another \p coo_matrix.
         *
         *  \param matrix Another \p coo_matrix with possibly different IndexType and ValueType.
         */
        template <typename IndexType2, typename ValueType2, typename MemorySpace2>
        coo_matrix& operator=(const coo_matrix<IndexType2, ValueType2, MemorySpace2>& matrix);

        /*! Assignment from another matrix format.
         *
         *  \param matrix Another sparse or dense matrix.
         */
        template <typename MatrixType>
        coo_matrix& operator=(const MatrixType& matrix);
    }; // class coo_matrix
/*! \}
 */

} // end namespace cusp

#include <cusp/detail/coo_matrix.inl>

