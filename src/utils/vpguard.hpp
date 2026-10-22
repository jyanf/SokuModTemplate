#pragma once
#include <Windows.h>
namespace utils {
    class VpGuard {
    public:
        inline VpGuard(void* address, SIZE_T size, DWORD newProtect = PAGE_EXECUTE_READWRITE) : _address(address), _size(size)
        {
            _success = ::VirtualProtect(_address, _size, newProtect, &_oldProtect) != FALSE;
        }
        VpGuard(DWORD address, SIZE_T size, DWORD newProtect = PAGE_EXECUTE_READWRITE) : VpGuard((void*)address, size, newProtect) {}

        ~VpGuard() {
            if (_success) {
                DWORD ignored;
                ::VirtualProtect(_address, _size, _oldProtect, &ignored);
            }
        }

        VpGuard(const VpGuard&) = delete;
        VpGuard& operator=(const VpGuard&) = delete;

        // check result
        explicit operator bool() const noexcept {
            return _success;
        }

    private:
        void* const _address;
        const SIZE_T _size;
        DWORD  _oldProtect = 0;
        bool   _success = false;
    };
}