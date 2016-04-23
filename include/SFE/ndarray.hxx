#ifndef SFE_NDARRAY_HXX
#define SFE_NDARRAY_HXX

#include <vector>

namespace sfe
{
    ////////////////////////////////////////////////////////////
    /// Container for 2-dimensional arrays.
    ////////////////////////////////////////////////////////////
    template <typename T>
    class Array2D
    {
    public:

        ////////////////////////////////////////////////////////////
        /// Type of the stored elements.
        ////////////////////////////////////////////////////////////
        typedef T value_type;

        ////////////////////////////////////////////////////////////
        /// Reference type of the stored elements.
        ////////////////////////////////////////////////////////////
        typedef value_type & reference;

        ////////////////////////////////////////////////////////////
        /// Constant reference type of the stored elements.
        ////////////////////////////////////////////////////////////
        typedef value_type const & const_reference;

        ////////////////////////////////////////////////////////////
        /// Iterator type.
        ////////////////////////////////////////////////////////////
        typedef typename std::vector<value_type>::iterator iterator;

        ////////////////////////////////////////////////////////////
        /// Constant iterator type.
        ////////////////////////////////////////////////////////////
        typedef typename std::vector<value_type>::const_iterator const_iterator;

        ////////////////////////////////////////////////////////////
        /// Create an array of the given size and initialize it with
        /// the given value.
        ////////////////////////////////////////////////////////////
        explicit Array2D(size_t width = 0, size_t height = 0, const_reference init = value_type());

        ////////////////////////////////////////////////////////////
        /// Resize the array.
        ////////////////////////////////////////////////////////////
        void resize(size_t width, size_t height, const_reference val = value_type());

        ////////////////////////////////////////////////////////////
        /// Return the width.
        ////////////////////////////////////////////////////////////
        size_t width() const;

        ////////////////////////////////////////////////////////////
        /// Return the height.
        ////////////////////////////////////////////////////////////
        size_t height() const;

        ////////////////////////////////////////////////////////////
        /// Access the element at (x, y).
        ////////////////////////////////////////////////////////////
        reference operator()(size_t x, size_t y);

        ////////////////////////////////////////////////////////////
        /// Access the element at (x, y).
        ////////////////////////////////////////////////////////////
        const_reference operator()(size_t x, size_t y) const;

        ////////////////////////////////////////////////////////////
        /// Return the first element, the element at (0, 0).
        ////////////////////////////////////////////////////////////
        reference front();

        ////////////////////////////////////////////////////////////
        /// Return the first element, the element at (0, 0).
        const_reference front() const;

        ////////////////////////////////////////////////////////////
        /// Return the last element, the element at
        /// (width-1, height-1).
        ////////////////////////////////////////////////////////////
        reference back();

        ////////////////////////////////////////////////////////////
        /// Return the last element, the element at
        /// (width-1, height-1).
        ////////////////////////////////////////////////////////////
        const_reference back() const;

        ////////////////////////////////////////////////////////////
        /// Return an iterator that points to the begin of the
        /// stored data.
        ////////////////////////////////////////////////////////////
        iterator begin();

        ////////////////////////////////////////////////////////////
        /// Return an iterator that points to the begin of the
        /// stored data.
        ////////////////////////////////////////////////////////////
        const_iterator begin() const;

        ////////////////////////////////////////////////////////////
        /// Return an iterator that points to the begin of the
        /// stored data.
        ////////////////////////////////////////////////////////////
        const_iterator cbegin() const;

        ////////////////////////////////////////////////////////////
        /// Return an iterator that points to the end of the stored
        /// data.
        ////////////////////////////////////////////////////////////
        iterator end();

        ////////////////////////////////////////////////////////////
        /// Return an iterator that points to the end of the stored
        /// data.
        ////////////////////////////////////////////////////////////
        const_iterator end() const;

        ////////////////////////////////////////////////////////////
        /// Return an iterator that points to the end of the stored
        /// data.
        ////////////////////////////////////////////////////////////
        const_iterator cend() const;

    private:

        ////////////////////////////////////////////////////////////
        /// The container for all stored elements.
        ////////////////////////////////////////////////////////////
        std::vector<value_type> data_;

        ////////////////////////////////////////////////////////////
        /// The width.
        ////////////////////////////////////////////////////////////
        size_t width_;

        ////////////////////////////////////////////////////////////
        /// The height.
        ////////////////////////////////////////////////////////////
        size_t height_;

    }; // class Array2D

    template <typename T>
    Array2D<T>::Array2D(size_t width, size_t height, const_reference init)
        :
        data_(width*height, init),
        width_(width),
        height_(height)
    {}

    template <typename T>
    void Array2D<T>::resize(size_t width, size_t height, const_reference val)
    {
        data_.resize(width*height, val);
        width_ = width;
        height_ = height;
    }

    template <typename T>
    size_t Array2D<T>::width() const
    {
        return width_;
    }

    template <typename T>
    size_t Array2D<T>::height() const
    {
        return height_;
    }

    template <typename T>
    typename Array2D<T>::reference Array2D<T>::operator()(size_t x, size_t y)
    {
        return data_[y*width_ + x];
    }

    template <typename T>
    typename Array2D<T>::const_reference Array2D<T>::operator()(size_t x, size_t y) const
    {
        return data_[y*width_ + x];
    }

    template <typename T>
    typename Array2D<T>::reference Array2D<T>::front()
    {
        return data_.front();
    }

    template <typename T>
    typename Array2D<T>::const_reference Array2D<T>::front() const
    {
        return data_.front();
    }

    template <typename T>
    typename Array2D<T>::reference Array2D<T>::back()
    {
        return data_.back();
    }

    template <typename T>
    typename Array2D<T>::const_reference Array2D<T>::back() const
    {
        return data_.back();
    }

    template <typename T>
    typename Array2D<T>::iterator Array2D<T>::begin()
    {
        return data_.begin();
    }

    template <typename T>
    typename Array2D<T>::const_iterator Array2D<T>::begin() const
    {
        return data_.begin();
    }

    template <typename T>
    typename Array2D<T>::const_iterator Array2D<T>::cbegin() const
    {
        return data_.cbegin();
    }

    template <typename T>
    typename Array2D<T>::iterator Array2D<T>::end()
    {
        return data_.end();
    }

    template <typename T>
    typename Array2D<T>::const_iterator Array2D<T>::end() const
    {
        return data_.end();
    }

    template <typename T>
    typename Array2D<T>::const_iterator Array2D<T>::cend() const
    {
        return data_.cend();
    }
}

#endif
