#pragma once

template <typename T>
struct get_container_type
{
	using type = void*;
};

template <typename T, template<typename, typename...> class V, typename... Args>
struct get_container_type<V<T, Args...>>
{
	using type = T;
};