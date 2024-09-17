#pragma once

namespace not_std
{
	// STRUCT TEMPLATE remove_reference
	template <class _Ty>
	struct remove_reference {
		using type = _Ty;
	};

	template <class _Ty>
	struct remove_reference<_Ty&> {
		using type = _Ty;
	};

	template <class _Ty>
	struct remove_reference<_Ty&&> {
		using type = _Ty;
	};

	template <class _Ty>
	using remove_reference_t = typename remove_reference<_Ty>::type;

	// STRUCT TEMPLATE remove_const
	template <class _Ty>
	struct remove_const { // remove top-level const qualifier
		using type = _Ty;
	};

	template <class _Ty>
	struct remove_const<const _Ty> {
		using type = _Ty;
	};

	template <class _Ty>
	using remove_const_t = typename remove_const<_Ty>::type;
}

namespace not_xor_not_ud
{

	template<class _Ty>
	using clean_type = typename not_std::remove_const_t<not_std::remove_reference_t<_Ty>>;

	template <int _size, char _key1, char _key2, typename T>
	class sk_crypter
	{
	public:
		constexpr sk_crypter(T* data)
		{
			crypt(data);
		}

		T* get()
		{
			return _storage;
		}

		int size() // (w)char count
		{
			return _size;
		}

		char key()
		{
			return _key1;
		}

		T* encrypt()
		{
			if (!is_encrypted())
				crypt(_storage);

			return _storage;
		}

		T* decrypt()
		{
			if (is_encrypted())
				crypt(_storage);

			return _storage;
		}

		bool is_encrypted()
		{
			return _storage[_size - 1] != 0;
		}

		void clear() // set full storage to 0
		{
			for (int i = 0; i < _size; i++)
			{
				_storage[i] = 0;
			}
		}

		operator T* ()
		{
			decrypt();

			return _storage;
		}

	private:
		constexpr void crypt(T* data)
		{
			for (int i = 0; i < _size; i++)
			{
				_storage[i] = data[i] ^ (_key1 + i % (1 + _key2));
			}
		}

		T _storage[_size]{ };
	};

	long sstrtol(const char* str, char** endptr, int base) {
		while (*str == ' ' || (*str >= '\t' && *str <= '\r')) {
			str++;
		}

		int negative = 0;
		if (*str == '-') {
			negative = 1;
			str++;
		}
		else if (*str == '+') {
			str++;
		}

		if (base == 0) {
			if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
				base = 16;
				str += 2;
			}
			else if (str[0] == '0') {
				base = 8;
				str++;
			}
			else {
				base = 10;
			}
		}

		long result = 0;
		while (1) {
			int digit;
			if (*str >= '0' && *str <= '9') {
				digit = *str - '0';
			}
			else if (*str >= 'a' && *str <= 'z') {
				digit = *str - 'a' + 10;
			}
			else if (*str >= 'A' && *str <= 'Z') {
				digit = *str - 'A' + 10;
			}
			else {
				break;
			}

			if (digit >= base) {
				break;
			}

			result = result * base + digit;
			str++;
		}

		if (endptr != 0) {
			*endptr = (char*)str;
		}

		return (negative) ? -result : result;
	}
}

#define sk_crypt(str) sk_crypter_key(str, __TIME__[5], __TIME__[7])
#define sk_crypter_key(str, key1, key2) []() { \
			constexpr static auto crypted_syr = not_xor_not_ud::sk_crypter \
				<sizeof(str) / sizeof(str[0]), key1, key2, not_xor_not_ud::clean_type<decltype(str[0])>>((not_xor_not_ud::clean_type<decltype(str[0])>*)str); \
					return crypted_syr; }()


//use offset("0x1239"); нет это более хороший сккрипт и offset это крипт офсетов
#define offset(str) not_xor_not_ud::sstrtol(sk_crypter_key(str, __TIME__[5], __TIME__[7]), NULL, 0)