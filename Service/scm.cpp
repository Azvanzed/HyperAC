#include <Windows.h>
#include <winsvc.h>

#include <scm.hpp>

bool scm::Create(const std::wstring& name, const std::wstring& description, const std::wstring& filepath, const uint32_t type) {
    SC_HANDLE scm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if (!scm) {
        return false;
    }

    SC_HANDLE service = OpenService(scm, name.data(), DELETE);
    if (service) {
        DeleteService(service);
        CloseServiceHandle(service);
    }

    service = CreateService(scm, name.data(), name.data(), SERVICE_ALL_ACCESS, type,
        SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, filepath.data(),
        nullptr, nullptr, nullptr, nullptr, nullptr);

    if (!service) {
        CloseServiceHandle(scm);
        return false;
    }

    if (!description.empty()) {
        SERVICE_DESCRIPTION desc;
        desc.lpDescription = (wchar_t*)description.data();
        ChangeServiceConfig2(service, SERVICE_CONFIG_DESCRIPTION, &desc);
    }

    CloseServiceHandle(service);
    CloseServiceHandle(scm);
    return true;
}

bool scm::Start(const std::wstring& name) {
    const SC_HANDLE scm = OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);
    if (!scm) {
        return false;
    }

    const SC_HANDLE service = OpenService(scm, name.data(), SERVICE_START);
    if (!service) {
        CloseServiceHandle(scm);
        return false;
    }

    const bool result = StartService(service, 0, nullptr);

    CloseServiceHandle(service);
    CloseServiceHandle(scm);
    return result;
}

bool scm::Destroy(const std::wstring& name) {
    SC_HANDLE scm = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
    if (!scm) {
        return false;
    }

    SC_HANDLE service = OpenService(scm, name.data(), SERVICE_STOP | DELETE | SERVICE_QUERY_STATUS);
    if (!service) {
        CloseServiceHandle(scm);
        return false;
    }

    SERVICE_STATUS status;
    memset(&status, 0, sizeof(SERVICE_STATUS));

    if (ControlService(service, SERVICE_CONTROL_STOP, &status)) {
        while (QueryServiceStatus(service, &status)) {
            if (status.dwCurrentState == SERVICE_STOP_PENDING) {
                Sleep(500);
            }
            else {
                break;
            }
        }
    }

    if (status.dwCurrentState != SERVICE_STOPPED) {
        CloseServiceHandle(service);
        CloseServiceHandle(scm);
        return false;
    }

    bool result = DeleteService(service);
    Sleep(500);

    CloseServiceHandle(service);
    CloseServiceHandle(scm);

    return result;
}