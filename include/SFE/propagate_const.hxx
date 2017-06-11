#ifndef SFE_PROPAGATE_CONST_HXX
#define SFE_PROPAGATE_CONST_HXX

#include <SFE/sfestd.hxx>

#include <type_traits>
#include <utility>

namespace sfe
{
    template <typename T>
    class propagate_const;

    template <typename T>
    struct is_propagate_const : std::false_type
    {};

    template <typename T>
    struct is_propagate_const<propagate_const<T>> : std::true_type
    {};

    template <typename T>
    class propagate_const
    {
    public:

        static_assert(!std::is_array<T>::value, "propagate_const must not contain array type.");

        typedef typename std::remove_reference<decltype(*std::declval<T&>())>::type element_type;

    private:

        // Allow that propagate_const<U> can move from propagate_const<T>.
        template <typename U>
        friend class propagate_const;

        // Give get_underlying access to the stored pointer.
        template <typename U>
        friend U const& get_underlying(propagate_const<U> const& p) noexcept;

        // Give get_underlying access to the stored pointer.
        template <typename U>
        friend U& get_underlying(propagate_const<U> & p) noexcept;

        // Helper that returns a raw pointer from both raw and smart pointers.
        template <typename U>
        static element_type* to_raw_pointer(U* u)
        {
            return u;
        }

        // Helper that returns a raw pointer from both raw and smart pointers.
        template <typename U>
        static element_type* to_raw_pointer(U& u)
        {
            return u.get();
        }

        // Helper that returns a raw pointer from both raw and smart pointers.
        template <typename U>
        static element_type const* to_raw_pointer(U const* u)
        {
            return u;
        }

        // Helper that returns a raw pointer from both raw and smart pointers.
        template <typename U>
        static element_type const* to_raw_pointer(U const& u)
        {
            return u.get();
        }

    public:

        ////////////////////////////////////////////////////////////
        /// Default constructor.
        ////////////////////////////////////////////////////////////
        propagate_const() = default;

        ////////////////////////////////////////////////////////////
        /// Disable copy constructor.
        ////////////////////////////////////////////////////////////
        propagate_const(propagate_const const& other) = delete;

        ////////////////////////////////////////////////////////////
        /// Enable move constructor.
        ////////////////////////////////////////////////////////////
        propagate_const(propagate_const && other) = default;

        ////////////////////////////////////////////////////////////
        /// Move construct from propagate_const of different pointer
        /// type. This is implicit because U&& is convertible to T.
        ////////////////////////////////////////////////////////////
        template <
            typename U,
            typename std::enable_if<
                std::is_constructible<T, U&&>::value && std::is_convertible<U&&, T>::value,
                bool
            >::type = true
        >
        propagate_const(propagate_const<U> && other)
            :
            t_(std::move(other.t_))
        {}

        ////////////////////////////////////////////////////////////
        /// Move construct from propagate_const of different pointer
        /// type. This is explicit because U&& is not convertible to
        /// T.
        ////////////////////////////////////////////////////////////
        template <
            typename U,
            typename std::enable_if<
                std::is_constructible<T, U&&>::value && !std::is_convertible<U&&, T>::value,
                bool
            >::type = false
        >
        explicit propagate_const(propagate_const<U> && other)
            :
            t_(std::move(other.t_))
        {}

        ////////////////////////////////////////////////////////////
        /// Move construct from pointer type. This is implicit
        /// because U&& is convertible to T.
        ////////////////////////////////////////////////////////////
        template <
            typename U,
            typename std::enable_if<
                std::is_constructible<T, U&&>::value && std::is_convertible<U&&, T>::value && !is_propagate_const<typename std::decay<U>::type>::value,
                bool
            >::type = true
        >
        propagate_const(U && u)
            :
            t_(std::forward<U>(u))
        {}

        ////////////////////////////////////////////////////////////
        /// Move construct from pointer type. This is explicit
        /// because U&& is not convertible to T.
        ////////////////////////////////////////////////////////////
        template <
            typename U,
            typename std::enable_if<
                std::is_constructible<T, U&&>::value && !std::is_convertible<U&&, T>::value && !is_propagate_const<typename std::decay<U>::type>::value,
                bool
            >::type = false
        >
        explicit propagate_const(U && u)
            :
            t_(std::forward<U>(u))
        {}

        ////////////////////////////////////////////////////////////
        /// Disable copy assignment.
        ////////////////////////////////////////////////////////////
        propagate_const & operator=(propagate_const& other) = delete;

        ////////////////////////////////////////////////////////////
        /// Enable move assignment.
        ////////////////////////////////////////////////////////////
        propagate_const & operator=(propagate_const && other) = default;

        ////////////////////////////////////////////////////////////
        /// Move assign from propagate_const of different pointer
        /// type.
        ////////////////////////////////////////////////////////////
        template <
            typename U,
            typename std::enable_if<std::is_convertible<U&&, T>::value>::type
        >
        propagate_const & operator=(propagate_const<U> && other)
        {
            t_ = std::move(other.t_);
        }

        ////////////////////////////////////////////////////////////
        /// Move assign from pointer type.
        ////////////////////////////////////////////////////////////
        template <
            typename U,
            typename std::enable_if<
                std::is_convertible<U&&, T>::value && !is_propagate_const<typename std::decay<U>::type>::value
            >::type
        >
            propagate_const & operator=(U && other)
        {
            t_ = std::forward<U>(other);
        }

        ////////////////////////////////////////////////////////////
        /// Casts the stored pointer to bool.
        //////////////////////////////////////////////////////////// 
        explicit operator bool() const
        {
            return static_cast<bool>(t_);
        }

        ////////////////////////////////////////////////////////////
        /// Returns the stored pointer.
        ////////////////////////////////////////////////////////////
        element_type const* operator->() const
        {
            return get();
        }

        ////////////////////////////////////////////////////////////
        /// Returns the stored pointer.
        ////////////////////////////////////////////////////////////
        template <
            typename U = T,
            typename std::enable_if<
                std::is_pointer<U>::value || std::is_convertible<U, element_type const*>::value,
                bool
            >::type = true
        >
        operator element_type const*() const
        {
            return get();
        }

        ////////////////////////////////////////////////////////////
        /// Dereferences the stored pointer.
        ////////////////////////////////////////////////////////////
        element_type const& operator*() const
        {
            return *get();
        }

        ////////////////////////////////////////////////////////////
        /// Returns the stored pointer.
        ////////////////////////////////////////////////////////////
        element_type const* get() const
        {
            return to_raw_pointer(t_);
        }

        ////////////////////////////////////////////////////////////
        /// Returns the stored pointer.
        ////////////////////////////////////////////////////////////
        element_type* operator->()
        {
            return get();
        }

        ////////////////////////////////////////////////////////////
        /// Returns the stored pointer.
        ////////////////////////////////////////////////////////////
        template <
            typename U = T,
            typename std::enable_if<
                std::is_pointer<U>::value || std::is_convertible<U, element_type*>::value,
                bool
            >::type = true
        >
        operator element_type*()
        {
            return get();
        }

        ////////////////////////////////////////////////////////////
        /// Dereferences the stored pointer.
        ////////////////////////////////////////////////////////////
        element_type& operator*()
        {
            return *get();
        }

        ////////////////////////////////////////////////////////////
        /// Returns the stored pointer.
        ////////////////////////////////////////////////////////////
        element_type* get()
        {
            return to_raw_pointer(t_);
        }

        ////////////////////////////////////////////////////////////
        /// Swaps the stored pointers.
        ////////////////////////////////////////////////////////////
        void swap(propagate_const & pt) noexcept  // FIXME PS 20170611: Must be noexcept(std::is_nothrow_swappable<T>::value), but this type trait does not exist, yet.
        {
            using std::swap;
            swap(t_, pt.t_);
        }

    private:

        ////////////////////////////////////////////////////////////
        /// The stored (maybe smart) pointer type.
        ////////////////////////////////////////////////////////////
        T t_;

    }; // propagate_const

    template <typename T>
    T const& get_underlying(propagate_const<T> const& p) noexcept
    {
        return p.t_;
    }

    template <typename T>
    T & get_underlying(propagate_const<T> & p) noexcept
    {
        return p.t_;
    }

    template <typename T>
    bool operator==(std::nullptr_t, propagate_const<T> const& p)
    {
        return nullptr == get_underlying(p);
    }

    template <typename T>
    bool operator==(propagate_const<T> const& p, std::nullptr_t)
    {
        return get_underlying(p) == nullptr;
    }

    template <typename T>
    bool operator!=(std::nullptr_t, propagate_const<T> const& p)
    {
        return nullptr != get_underlying(p);
    }

    template <typename T>
    bool operator!=(propagate_const<T> const& p, std::nullptr_t)
    {
        return get_underlying(p) != nullptr;
    }

    template <typename T, typename U>
    bool operator==(propagate_const<T> const& t, propagate_const<U> const& u)
    {
        return get_underlying(t) == get_underlying(u);
    }

    template <typename T, typename U>
    bool operator!=(propagate_const<T> const& t, propagate_const<U> const& u)
    {
        return get_underlying(t) != get_underlying(u);
    }

    template <typename T, typename U>
    bool operator<(propagate_const<T> const& t, propagate_const<U> const& u)
    {
        return get_underlying(t) < get_underlying(u);
    }

    template <typename T, typename U>
    bool operator>(propagate_const<T> const& t, propagate_const<U> const& u)
    {
        return get_underlying(t) > get_underlying(u);
    }

    template <typename T, typename U>
    bool operator<=(propagate_const<T> const& t, propagate_const<U> const& u)
    {
        return get_underlying(t) <= get_underlying(u);
    }

    template <typename T, typename U>
    bool operator>=(propagate_const<T> const& t, propagate_const<U> const& u)
    {
        return get_underlying(t) >= get_underlying(u);
    }

    template <typename T, typename U>
    bool operator==(propagate_const<T> const& t, U const& u)
    {
        return get_underlying(t) == u;
    }

    template <typename T, typename U>
    bool operator!=(propagate_const<T> const& t, U const& u)
    {
        return get_underlying(t) != u;
    }

    template <typename T, typename U>
    bool operator<(propagate_const<T> const& t, U const& u)
    {
        return get_underlying(t) < u;
    }

    template <typename T, typename U>
    bool operator>(propagate_const<T> const& t, U const& u)
    {
        return get_underlying(t) > u;
    }

    template <typename T, typename U>
    bool operator<=(propagate_const<T> const& t, U const& u)
    {
        return get_underlying(t) <= u;
    }

    template <typename T, typename U>
    bool operator>=(propagate_const<T> const& t, U const& u)
    {
        return get_underlying(t) >= u;
    }

    template <typename T, typename U>
    bool operator==(T const& t, propagate_const<U> const& u)
    {
        return t == get_underlying(u);
    }

    template <typename T, typename U>
    bool operator!=(T const& t, propagate_const<U> const& u)
    {
        return t != get_underlying(u);
    }

    template <typename T, typename U>
    bool operator<(T const& t, propagate_const<U> const& u)
    {
        return t < get_underlying(u);
    }

    template <typename T, typename U>
    bool operator>(T const& t, propagate_const<U> const& u)
    {
        return t > get_underlying(u);
    }

    template <typename T, typename U>
    bool operator<=(T const& t, propagate_const<U> const& u)
    {
        return t <= get_underlying(u);
    }

    template <typename T, typename U>
    bool operator>=(T const& t, propagate_const<U> const& u)
    {
        return t >= get_underlying(u);
    }

    template <typename T>
    void swap(propagate_const<T> & t, propagate_const<T> & u) noexcept  // FIXME PS 20170611: Must be noexcept(std::is_nothrow_swappable<T>::value), but this type trait does not exist, yet.
    {
        t.swap(u);
    }

} // namespace sfe

#endif
