#include "src/overlay.h"
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include "icl.h"
#include "proc.h"

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")



//Base by iraizo on github: https://github.com/iraizo/nvidia-overlay-hijack


namespace Data {
    bool ShouldUpdate = false;

    bool InfiniteNitro = false;
    bool InfiniteNitroIni = false;

    bool InfiniteSpeed = false;
    bool InfiniteSpeedIni = false;

    bool EditCredits = false;
    bool EditCurrencies = false;

    bool LapTimeEdit = false;
    bool LapTimeEditini = false;

    bool FovChanger = false;
    bool FovChangerIni = false;
    bool ForceFovChange = false;

    bool StopEntities = false;

    bool Exit = false;
    bool Debug = false;

    ULONGLONG CountRefreshBase = 0;
    ULONGLONG CountRefreshEntities = 0;
    ULONGLONG CountRefreshSpeed = 0;
    ULONGLONG CountRefreshSetFov = 0;

    DWORD ProcID = 0;
    HANDLE ProcHandle = 0;
    uintptr_t NitroOffset = 0x0;
    uintptr_t NitroAddress = 0x0;
    uintptr_t SpeedCopy0Offset = 0x0;
    uintptr_t SpeedCopy1Offset = 0x0;
    uintptr_t SpeedCopy2Offset = 0x0;
    uintptr_t SpeedCopy3Offset = 0x0;
    uintptr_t SpeedCopy4Offset = 0x0;
    uintptr_t LapTimeOffset = 0x0;
    uintptr_t SpeedOffsetPtr = 0x0;
    uintptr_t CurrencyPtr = 0x0;

    uintptr_t FovFunctionOffset = 0x0;

    uintptr_t WorldOffset = 0x0;
    uintptr_t WorldPtr = 0x0;


    uintptr_t EntityListOffset = 0x0;
    uintptr_t ViewportOffset = 0x0;
    uintptr_t LocalPlayerCar = 0x0;

    float SpeedLimit = 5000;
    float Fov = 100.0f;

    uintptr_t GameBase = 0x0;

    //std::vector<unsigned int>SpeedOffset = { 0xC28, 0xA80, 0x70 , 0x60, 0x188, 0 };
    std::vector<unsigned int>ClassOfCarOffsets = { 0x10, 0x10, 0x568, 0x8, 0x10, 0x70, 0x0 };
    std::vector<unsigned int>SpeedOffset = { 0xC28, 0xA80, 0x30 , 0xD0, 0x60, 0x188, 0x0 };
    std::vector<unsigned int>CredsOffset = { 0x50, 0x610 };

    std::vector<class BYTENOPED*> ByteList;
    const char* Catch = "OOPS";
    

    class BYTENOPED //Error: 5035 -> throws if too many functions were NOPED
    {
    public:
        BYTENOPED(char* BYTES, uintptr_t Address, size_t BYTELENGTH) {
            if (ByteList.size() > 1000)throw std::runtime_error("Created too many Hook Templates! (You probly didnt check for shit my guy)");
            this->BYTES = BYTES;
            this->Address = Address;
            this->CharLength = BYTELENGTH;
        }

        int GetIndex(uintptr_t Address) {

            for (size_t i = 0; i < ByteList.size(); i++)
            {
                if (Address == ByteList[i]->Address)return i;
            }
        }

        int GetSize() {
            return this->CharLength;
        }

        char* GetBYTEs() {
            if (this->BYTES == nullptr) {
                this->CharLength = 0;
                return (char*)Catch;
            }
            return this->BYTES;
        }

        void FreeChar() {
            if (this->BYTES == nullptr)return;
            free(this->BYTES);
        }

    private:
        size_t CharLength = 0;
        char* BYTES = nullptr;
        uintptr_t Address = 0x0;

    };



#include "SigShit.h"

    void FindSigs() {
        uintptr_t DSize = 0;
        SigShit::GetProcessMemoryInfoeX64(Data::ProcHandle, &DSize);

        LPCSTR Signature = "\x33\x0d\x00\x00\x00\x00\x41\x89\x08\x41\x8b\xc8";
        LPCSTR Mask = "xx????xxxxxx";

        NewBugatti1 = { Data::ProcHandle , Data::GameBase, DSize, Signature, Mask, &Data::NitroOffset };
        NewBugatti1.ModuleName = "Nitro_switch_Value_Function";
        NewBugatti1.EditAddrs = -0x5A;
        NewBugatti1.ReadMem = false;
        NewBugatti1.MinusBase = true;
        NewBugatti1.SkipFirstFind = false;
        NewBugatti1.GoToAddressFromStart = true;
        SigShit::DebugPrint64(NewBugatti1);
        NewBugatti1.GoToAddressFromStart = false;


        Signature = "\x48\x8b\x01\x74\x00\xff\x90\x00\x00\x00\x00\x48\x8b\x4b";
        Mask = "xxxx?xx????xxx";
        NewBugatti1 = { Data::ProcHandle , Data::GameBase, DSize, Signature, Mask, &Data::SpeedCopy0Offset };
        NewBugatti1.ModuleName = "SpeedCopyFunc";
        NewBugatti1.EditAddrs = -0x34;
        NewBugatti1.ReadMem = false;
        NewBugatti1.MinusBase = true;
        NewBugatti1.SkipFirstFind = false;
        NewBugatti1.GoToAddressFromStart = true;
        SigShit::DebugPrint64(NewBugatti1);
        NewBugatti1.GoToAddressFromStart = false;


        Data::SpeedCopy0Offset += 0x1;

        Data::SpeedCopy1Offset = Data::SpeedCopy0Offset + 0x5;
        Data::SpeedCopy2Offset = Data::SpeedCopy1Offset + 0x5;




        Signature = "\xf3\x0f\x5e\xc8\x0f\x28\xc3\xf3\x0f\x10\x59";
        Mask = "xxxxxxxxxxx";
        NewBugatti1 = { Data::ProcHandle , Data::GameBase, DSize, Signature, Mask, &Data::SpeedCopy3Offset };
        NewBugatti1.ModuleName = "SpeedCopyFunc2";
        NewBugatti1.EditAddrs = -0x8B;
        NewBugatti1.ReadMem = false;
        NewBugatti1.MinusBase = true;
        NewBugatti1.SkipFirstFind = false;
        NewBugatti1.GoToAddressFromStart = true;
        SigShit::DebugPrint64(NewBugatti1);
        NewBugatti1.GoToAddressFromStart = false;

        Data::SpeedCopy4Offset = Data::SpeedCopy3Offset + 0x60;



        Signature = "\x48\x8b\x05\x00\x00\x00\x00\x48\x8b\x98\x00\x00\x00\x00\x48\x8b\xcb\xe8\x00\x00\x00\x00\x0a\x83\x00\x00\x00\x00\x0a\x43\x00\x74\x00\x48\x8b\xcb\xe8\x00\x00\x00\x00\xf3\x0f\x10\x55";
        Mask = "xxx????xxx????xxxx????xx????xx?x?xxxx????xxxx";

        NewBugatti1 = { Data::ProcHandle , Data::GameBase, DSize, Signature, Mask, &Data::SpeedOffsetPtr };
        NewBugatti1.ModuleName = "Engine_Pointer";
        NewBugatti1.EditAddrs = 0x2;
        NewBugatti1.ReadMem = true;
        NewBugatti1.MinusBase = true;
        SigShit::ReadAddrbackwards = true;
        SigShit::DebugPrint64(NewBugatti1);
        SigShit::ReadAddrbackwards = false;



        Signature = "\x48\x8b\x0d\x00\x00\x00\x00\xe8\x00\x00\x00\x00\x44\x2b\xf6";
        Mask = "xxx????x????xxx";

        NewBugatti1 = { Data::ProcHandle , Data::GameBase, DSize, Signature, Mask, &Data::CurrencyPtr };
        NewBugatti1.ModuleName = "Credit_Pointer";
        NewBugatti1.EditAddrs = 0x2;
        NewBugatti1.ReadMem = true;
        NewBugatti1.MinusBase = true;
        SigShit::ReadAddrbackwards = true;
        SigShit::DebugPrint64(NewBugatti1);
        SigShit::ReadAddrbackwards = false;



        Signature = "\x0f\x11\x4c\x01\x00\x45\x3b\xfd";
        Mask = "xxxx?xxx";

        NewBugatti1 = { Data::ProcHandle , Data::GameBase, DSize, Signature, Mask, &Data::LapTimeOffset };
        NewBugatti1.ModuleName = "Laptime_Function";
        NewBugatti1.EditAddrs = 0x0;
        NewBugatti1.ReadMem = false;
        NewBugatti1.MinusBase = true;
        NewBugatti1.SkipFirstFind = false;
        SigShit::DebugPrint64(NewBugatti1);
        NewBugatti1.SkipFirstFind = false;


        
        Signature = "\x48\x8b\x0d\x00\x00\x00\x00\xe8\x00\x00\x00\x00\x48\x8b\x0d\x00\x00\x00\x00\xb2\x00\xe8\x00\x00\x00\x00\x48\x8b\x0d\x00\x00\x00\x00\xb2";
        Mask = "xxx????x????xxx????x?x????xxx????x";

        NewBugatti1 = { Data::ProcHandle , Data::GameBase, DSize, Signature, Mask, &Data::WorldOffset };
        NewBugatti1.ModuleName = "World_Pointer";
        NewBugatti1.EditAddrs = 0x2;
        NewBugatti1.ReadMem = true;
        NewBugatti1.MinusBase = true;
        SigShit::ReadAddrbackwards = true;
        SigShit::DebugPrint64(NewBugatti1);
        SigShit::ReadAddrbackwards = false;



        Signature = "\xf3\x0f\x11\x49\x00\xc3\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\x48\x89\x5c\x24\x00\x57\x48\x83\xec\x00\x48\x8b\xf9";
        Mask = "xxxx?xxxxxxxxxxxxxxx?xxxx?xxx";

        NewBugatti1 = { Data::ProcHandle , Data::GameBase, DSize, Signature, Mask, &Data::FovFunctionOffset };
        NewBugatti1.ModuleName = "Set_Fov_Function";
        NewBugatti1.EditAddrs = 0x0;
        NewBugatti1.ReadMem = false;
        NewBugatti1.MinusBase = true;
        SigShit::DebugPrint64(NewBugatti1);




        Signature = "\x0a\x45\x00\x74\x00\x48\x8b\xcd\xe8\x00\x00\x00\x00\x48\x8b\x0d";
        Mask = "xx?x?xxxx????xxx";

        NewBugatti1 = { Data::ProcHandle , Data::GameBase, DSize, Signature, Mask, &Data::EntityListOffset };
        NewBugatti1.ModuleName = "EntityList_Pointer";
        NewBugatti1.EditAddrs = 0xF;
        NewBugatti1.EditAddrdif = 0x2;
        NewBugatti1.ReadMem = true;
        NewBugatti1.MinusBase = true;
        SigShit::ReadAddrbackwards = true;
        SigShit::DebugPrint64(NewBugatti1);
        SigShit::ReadAddrbackwards = false;




        //WorldPointerOffset
        Signature = "\x48\x8b\x0d\x00\x00\x00\x00\xe8\x00\x00\x00\x00\x48\x8b\xd8\x49\x8b\x16";
        Mask = "xxx????x????xxxxxx";

        NewBugatti1 = { Data::ProcHandle , Data::GameBase, DSize, Signature, Mask, &Data::ViewportOffset };
        NewBugatti1.ModuleName = "Viewport_Pointer";
        NewBugatti1.EditAddrs = 0x2;
        NewBugatti1.ReadMem = true;
        NewBugatti1.MinusBase = true;
        SigShit::ReadAddrbackwards = true;
        SigShit::DebugPrint64(NewBugatti1);
        SigShit::ReadAddrbackwards = false;




    }


    bool NOPFunc(uintptr_t Address, HANDLE ProcHandle, int BYTESTONOP, bool UnNop = false) {

        if (UnNop) {

            if (ByteList[0] == nullptr || ByteList.size() == 0)return false;

            int Index = ByteList[0]->GetIndex(Address);
            char* BYTEARRAY = ByteList[Index]->GetBYTEs();

            char* ByteArrayFF = new char[ByteList[Index]->GetSize()];

            for (size_t i = 0; i < ByteList[Index]->GetSize(); i++)
            {
                if (!WriteProcessMemory(ProcHandle, (BYTE*)Address + i, &BYTEARRAY[i], sizeof(BYTEARRAY[i]), 0))break;
            }

            ByteList[Index]->FreeChar();



            free(ByteList[Index]);

            auto it = ByteList.begin();

            for (it = ByteList.begin(); it != ByteList.end();)
            {
                if ((*it) == ByteList[Index])break;
                ++it;
            }

            ByteList.erase(it);

            return true;
        }

        char NOPBYTE = 0x90;

        char* BYTES = new char[BYTESTONOP];

        if (BYTES == nullptr)return false;

        for (size_t i = 0; i < BYTESTONOP; i++)
        {
            if (!ReadProcessMemory(ProcHandle, (BYTE*)Address + i, &BYTES[i], sizeof(BYTES[i]), 0))break;
            if (!WriteProcessMemory(ProcHandle, (BYTE*)Address + i, &NOPBYTE, sizeof(NOPBYTE), 0))break;
        }



        ByteList.push_back(new BYTENOPED(BYTES, Address, BYTESTONOP));


        //delete[] BYTES;

        return true;
    }


    DWORD GetMemoryProtection(LPVOID address, HANDLE ProcHandle)
    {
        MEMORY_BASIC_INFORMATION memInfo;
        VirtualQueryEx(ProcHandle, address, &memInfo, sizeof(memInfo));
        return memInfo.Protect;
    }


    template<typename T3>
    bool ReadMemory(uintptr_t Address, T3& Value, T3 ValueF, HANDLE ProcHandle) {
        uintptr_t AddressV = (uintptr_t)Value;
        if (ReadProcessMemory(Data::ProcHandle, (BYTE*)Address, (LPVOID)AddressV, sizeof(ValueF), 0))return true;

        return false;
    }


    template<typename T4>
    bool WriteMemory(uintptr_t Address, T4 &ValueBuffer1, T4 Value, HANDLE ProcHandle) {
        uintptr_t Address1 = (uintptr_t)ValueBuffer1;
        if (Address == 0x0 || Address > 0x7fffffffffff || GetMemoryProtection(reinterpret_cast<LPVOID>(Address), ProcHandle) != PAGE_READWRITE) return false;

        if (WriteProcessMemory(Data::ProcHandle, (BYTE*)Address, (LPCVOID)Address1, sizeof(Value), 0))return true;

        return false;
    }


    bool ShouldUsePointer(uintptr_t Address, HANDLE ProcHandle) {
        if (Address == 0x0 || Address > 0x7fffffffffff || GetMemoryProtection(reinterpret_cast<LPVOID>(Address), ProcHandle) != PAGE_READWRITE) return false;

        return true;
    }



    std::wstring stringToWideString(const std::string& str)
    {
        std::wstring wstr(str.length(), L' ');
        std::copy(str.begin(), str.end(), wstr.begin());
        return wstr;
    }



    namespace CheatFunctions {
        //Include Shit
#include <Lmcons.h>


        //Data Shit
        std::vector<std::thread*> ThreadList;
        std::vector<uintptr_t> EntityList;
        std::thread *CleanThr;

        template<typename Function>
        std::thread* CreateThread_s(Function* Location) {

            for (size_t i = 0; i < ThreadList.size(); i++)
            {
                if (ThreadList[i] == nullptr || ThreadList[i]->joinable())continue;


                std::thread* TempPtr = ThreadList[i];
                delete TempPtr;
            }

            ThreadList.clear();


            std::thread* NewThread = new std::thread(Location);
            ThreadList.push_back(NewThread);

            return NewThread;
        }





        //Functions


        void CleanBanFiles() {
            
            if (CleanThr == nullptr) {
                CleanThr = CreateThread_s(CleanBanFiles);
                return;
            }

            if (std::this_thread::get_id() != CleanThr->get_id())return;
            

            if (MessageBoxW(NULL, L"You will have to delete them yourself, right click, press Security, then change to make yourself Owner then delete", L"?????????", MB_YESNO | MB_SYSTEMMODAL) == 6) {

                //Get Username and delete Folder Asphalt8 thats saving data
                if (ProcHandle != NULL)
                {
                    TerminateProcess(ProcHandle, 0);
                }

                char username[UNLEN + 1]; DWORD username_len = UNLEN + 1; GetUserNameA(username, &username_len); int Length = sizeof(username); std::string Amongus; Amongus = "C:\\Users\\"; Amongus = Amongus + username; Amongus = Amongus + "\\AppData\\Local\\Packages\\GAMELOFTSA.Asphalt8Airborne_0pp20fcewvvtj";

                std::remove(Amongus.c_str());

                system(std::string("del /S /F /Q " + Amongus).c_str());


                system("control.exe keymgr.dll");
                MessageBoxW(NULL, L"Next find the Webcredential with Asphalt8 in the name and delete it", L"?????????", MB_YESNO | MB_SYSTEMMODAL);

            }

            CleanThr = 0x0;
        }




        void EntityLoop() {
            unsigned int OffsetEntitties = 0x8;

            std::vector<unsigned int> Offset = { 0x20, 0x78, 0x20 , OffsetEntitties , 0x490, 0x0 };

            uintptr_t EntityBase = FindDMAAddy(Data::ProcHandle, Data::GameBase + Data::EntityListOffset, Offset);

            EntityList.clear();

            for (size_t i = 0; i < 500; i++)
            {

                EntityBase = FindDMAAddy(Data::ProcHandle, Data::GameBase + Data::EntityListOffset, Offset);

                OffsetEntitties += 0x8;

                Offset = { 0x20, 0x78, 0x20 , OffsetEntitties , 0x490, 0x0 };

                if (EntityBase == Data::LocalPlayerCar)continue;

                if (!ShouldUsePointer(EntityBase, Data::ProcHandle))break;

                EntityList.push_back(EntityBase);
            }

        }
    }
}




void Train() {
    if (Data::Exit) {
        //Nitro ReturnBytes
        {
            Data::NOPFunc(Data::NitroAddress, Data::ProcHandle, 3, true);
        }


        //SpeedShit ReturnBytes
        {
            //Data::NOPFunc(Data::GameBase + Data::SpeedCopy0Offset, Data::ProcHandle, 5, true);
            Data::NOPFunc(Data::GameBase + Data::SpeedCopy1Offset, Data::ProcHandle, 5, true);
           // Data::NOPFunc(Data::GameBase + Data::SpeedCopy2Offset, Data::ProcHandle, 5, true);
            Data::NOPFunc(Data::GameBase + Data::SpeedCopy3Offset, Data::ProcHandle, 5, true);
            Data::NOPFunc(Data::GameBase + Data::SpeedCopy4Offset, Data::ProcHandle, 5, true);
        }


        //Laptime ReturnBytes
        {
            Data::NOPFunc(Data::GameBase + Data::LapTimeOffset, Data::ProcHandle, 5, true);
        }


        //Fov ReturnBytes
        {
            Data::NOPFunc(Data::GameBase + Data::FovFunctionOffset, Data::ProcHandle, 5, true);
        }

    }



    if (GetTickCount64() > Data::CountRefreshBase) {
        Data::CountRefreshBase = GetTickCount64() + 2000;
        DWORD ProcID = GetProcId(L"Asphalt8.exe");
        if (ProcID == 0) {
            Data::ShouldUpdate = false;
            return;
        }
       

        if (ProcID != Data::ProcID) {
            Data::ProcID = ProcID;
            Data::ProcHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, ProcID);
            Data::GameBase = GetModuleBaseAddress(ProcID, L"Asphalt8.exe");
            if (Data::GameBase == 0x0) {
                Data::ShouldUpdate = false;
                return;
            }


            //Update GameStuff Here!
            Data::FindSigs();

            Data::NitroAddress = Data::GameBase + Data::NitroOffset;
            Data::WorldPtr = Data::GameBase + Data::WorldOffset;
        }

        Data::LocalPlayerCar = FindDMAAddy(Data::ProcHandle, Data::GameBase + Data::ViewportOffset, Data::ClassOfCarOffsets);

        Data::ShouldUpdate = true;
    }


    if (Data::ShouldUpdate) {

        if (Data::InfiniteNitro == true) {
            if (Data::InfiniteNitroIni == false) {

                Data::NOPFunc(Data::NitroAddress, Data::ProcHandle, 3);
                Data::InfiniteNitroIni = true;
            }
        }
        else
        {
            if (Data::InfiniteNitroIni == true) {
                Data::InfiniteNitroIni = false;

                Data::NOPFunc(Data::NitroAddress, Data::ProcHandle, 3, true);

            }
        }

        if(GetTickCount64() > Data::CountRefreshEntities)
        {
            Data::CountRefreshEntities = GetTickCount64() + 1000;

            Data::CheatFunctions::EntityLoop();
        }


        if (Data::InfiniteSpeed) {
            if (Data::InfiniteSpeedIni == false) {
                Data::InfiniteSpeedIni = true;
                //Data::NOPFunc(Data::GameBase + Data::SpeedCopy0Offset, Data::ProcHandle, 5);
                Data::NOPFunc(Data::GameBase + Data::SpeedCopy1Offset, Data::ProcHandle, 5);
                //Data::NOPFunc(Data::GameBase + Data::SpeedCopy2Offset, Data::ProcHandle, 5);
                Data::NOPFunc(Data::GameBase + Data::SpeedCopy3Offset, Data::ProcHandle, 5);
                Data::NOPFunc(Data::GameBase + Data::SpeedCopy4Offset, Data::ProcHandle, 5);
            }

            if (GetTickCount64() > Data::CountRefreshSpeed) {
                Data::CountRefreshSpeed = GetTickCount64() + 2000;

                uintptr_t AddressC = Data::GameBase + Data::SpeedOffsetPtr;

                Data::ReadMemory(AddressC, AddressC, AddressC, Data::ProcHandle);

                AddressC = FindDMAAddy(Data::ProcHandle, AddressC, Data::SpeedOffset);
                uintptr_t VTable = 0x0;
                if (ReadProcessMemory(Data::ProcHandle, (BYTE*)AddressC, &VTable, sizeof(VTable), 0) && Data::ShouldUsePointer(AddressC, Data::ProcHandle)) {

                    WriteProcessMemory(Data::ProcHandle, (BYTE*)AddressC + 0x20, &Data::SpeedLimit, sizeof(Data::SpeedLimit), 0);
                    WriteProcessMemory(Data::ProcHandle, (BYTE*)AddressC + 0x24, &Data::SpeedLimit, sizeof(Data::SpeedLimit), 0);
                    WriteProcessMemory(Data::ProcHandle, (BYTE*)AddressC + 0x28, &Data::SpeedLimit, sizeof(Data::SpeedLimit), 0);
                }
            }
        }
        else
        {
            if (Data::InfiniteSpeedIni == true) {
                Data::InfiniteSpeedIni = false;
                //Data::NOPFunc(Data::GameBase + Data::SpeedCopy0Offset, Data::ProcHandle, 5, true);
                Data::NOPFunc(Data::GameBase + Data::SpeedCopy1Offset, Data::ProcHandle, 5, true);
                //Data::NOPFunc(Data::GameBase + Data::SpeedCopy2Offset, Data::ProcHandle, 5, true);
                Data::NOPFunc(Data::GameBase + Data::SpeedCopy3Offset, Data::ProcHandle, 5, true);
                Data::NOPFunc(Data::GameBase + Data::SpeedCopy4Offset, Data::ProcHandle, 5, true);
                uintptr_t AddressC = Data::GameBase + Data::SpeedOffsetPtr;

                Data::ReadMemory(AddressC, AddressC, AddressC, Data::ProcHandle);

                AddressC = FindDMAAddy(Data::ProcHandle, AddressC, Data::SpeedOffset);
                uintptr_t VTable = 0x0;
                if (ReadProcessMemory(Data::ProcHandle, (BYTE*)AddressC, &VTable, sizeof(VTable), 0) && Data::ShouldUsePointer(AddressC, Data::ProcHandle)) {

                    float Speed = 201.0f;
                    WriteProcessMemory(Data::ProcHandle, (BYTE*)AddressC + 0x20, &Speed, sizeof(Speed), 0);
                    WriteProcessMemory(Data::ProcHandle, (BYTE*)AddressC + 0x24, &Speed, sizeof(Speed), 0);
                    WriteProcessMemory(Data::ProcHandle, (BYTE*)AddressC + 0x28, &Speed, sizeof(Speed), 0);
                }
            }
        }

        if (Data::EditCredits) {
            Data::EditCredits = false;

            //0xA0 is the current offset for Credits
            uintptr_t CurrencyObject = FindDMAAddy(Data::ProcHandle, Data::GameBase + Data::CurrencyPtr, Data::CredsOffset) + 0xA0;
            uintptr_t TempVal = 0x0;

            ReadProcessMemory(Data::ProcHandle, (BYTE*)CurrencyObject, &TempVal, sizeof(TempVal), 0);   TempVal -= 1000;

            VirtualProtectEx(Data::ProcHandle, (BYTE*)CurrencyObject, sizeof(TempVal), 0x40, 0);
            if (WriteProcessMemory(Data::ProcHandle, (BYTE*)CurrencyObject, &TempVal, sizeof(TempVal), 0) == false);
        }

        if (Data::EditCurrencies) {
            Data::EditCurrencies = false;

            uintptr_t CurrencyObject = FindDMAAddy(Data::ProcHandle, Data::GameBase + Data::CurrencyPtr, Data::CredsOffset);
            uintptr_t TempVal = 0x0;

            for (size_t i = 0, Data = 0; i < 40; i++)
            {
                ReadProcessMemory(Data::ProcHandle, (BYTE*)CurrencyObject + Data, &TempVal, sizeof(TempVal), 0);   TempVal -= 1000;

                VirtualProtectEx(Data::ProcHandle, (BYTE*)CurrencyObject + Data, sizeof(TempVal), 0x40, 0);
                if (WriteProcessMemory(Data::ProcHandle, (BYTE*)CurrencyObject + Data, &TempVal, sizeof(TempVal), 0) == false);

                Data += 0x8;
            }

            
        }


        if (Data::LapTimeEdit) {
            if (!Data::LapTimeEditini) {
                Data::LapTimeEditini = true;
                Data::NOPFunc(Data::GameBase + Data::LapTimeOffset, Data::ProcHandle, 5);
            }
        }
        else
        {
            if (Data::LapTimeEditini) {
                Data::LapTimeEditini = false;
                Data::NOPFunc(Data::GameBase + Data::LapTimeOffset, Data::ProcHandle, 5, true);
            }
        }



        if (Data::FovChanger) {
            if (!Data::FovChangerIni) {
                Data::FovChangerIni = true;
                Data::NOPFunc(Data::GameBase + Data::FovFunctionOffset, Data::ProcHandle, 5);

            }


            if (GetTickCount64() > Data::CountRefreshSetFov || Data::ForceFovChange) {
                Data::CountRefreshSetFov = GetTickCount64() + 600;
                Data::ForceFovChange = false;


                unsigned int Camera = 0x0;
                for (size_t i = 0; i < 40; i++)
                {

                    Camera = 0x10 * i;
                    Camera = Camera + 0x8;

                    std::vector<unsigned int>CameraOffsets = { 0x60, 0x100, Camera , 0x220,0x50 };

                    uintptr_t CurrentCamera = FindDMAAddy(Data::ProcHandle, Data::WorldPtr, CameraOffsets);
                    if (!Data::ShouldUsePointer(CurrentCamera, Data::ProcHandle))
                    {
                        continue;
                    }
                    float FovT = Data::Fov / 100;
                    WriteProcessMemory(Data::ProcHandle, (BYTE*)CurrentCamera, &FovT, sizeof(FovT), 0);

                }


            }

        }
        else
        {
            if (Data::FovChangerIni) {
                Data::FovChangerIni = false;
                Data::NOPFunc(Data::GameBase + Data::FovFunctionOffset, Data::ProcHandle, 5, true);

            }
        }


        if (Data::StopEntities) {
            float Velocity = 0.0f;
            for (size_t i = 0; i < Data::CheatFunctions::EntityList.size(); i++)
            {
                if (!Data::ShouldUsePointer(Data::CheatFunctions::EntityList[i] + 0x160, Data::ProcHandle))break;
                WriteProcessMemory(Data::ProcHandle, (BYTE*)Data::CheatFunctions::EntityList[i] + 0x160, &Velocity, sizeof(Velocity), 0);
                WriteProcessMemory(Data::ProcHandle, (BYTE*)Data::CheatFunctions::EntityList[i] + 0x164, &Velocity, sizeof(Velocity), 0);
                WriteProcessMemory(Data::ProcHandle, (BYTE*)Data::CheatFunctions::EntityList[i] + 0x168, &Velocity, sizeof(Velocity), 0);
            }
        }


    }

}




void render() {}



int main()
{


    Overlay* overlay = new Overlay(L"Alumni Sans Collegiate One", 28.0, 15.0);

    if (!overlay->init())
        return 1;
    if (!overlay->startup_d2d())
        return 1;

    bool DifText = false;
    bool Open = false;
    bool ShowWaterMark = true;
    float SomethingFF = 10.0f;

    HWND WindowHandle = GetDesktopWindow();

    RECT winRect;

    GetWindowRect(WindowHandle, &winRect);

    overlay->WindowSize.x = winRect.right;
    overlay->WindowSize.y = winRect.bottom;


    int Tab = 0;
    
    Data::ByteList.reserve(1);

    while (overlay->LoadingScreen() == false);

    while (!overlay->Quit && !GetAsyncKeyState(VK_DELETE))
    {
        

        GetCursorPos(&overlay->MousePosF);

        overlay->begin_scene();
        overlay->clear_scene();



        if (Open == true) {
            
            overlay->DrawMenu(); {

                //overlay->Spacing();
                switch (Tab)
                {
                case 0:
                    if (overlay->Button("Exit  ")) {
                        overlay->Quit = true;
                    }  

                    if (overlay->Button("Start Game  ")) {
                        {
                            if (system("explorer.exe shell:appsFolder\\GAMELOFTSA.Asphalt8Airborne_0pp20fcewvvtj!App") == 0 && system("explorer.exe shell:appsFolder\\Microsoft.GAMELOFTSA.Asphalt8Airborne_0pp20fcewvvtj!App") == 0)system("echo Couldnt find Asohalt8 && echo Starting Game... && cmd /c timeout /t 2 > nul");
                        }
                    }
                    if (overlay->Button("Misc Tab ")) {
                        Tab = 1;
                    }
                    if (overlay->Button("Random Credits"))Data::EditCredits = true;
                    overlay->Spacing();
                    overlay->CheckBox(R"(Lock "Window")", &overlay->LockWindow);
                    overlay->CheckBox("Show Watermark", &ShowWaterMark);
                    overlay->CheckBox("Infinite Nitro", &Data::InfiniteNitro);
                    overlay->CheckBox("Stop Timer", &Data::LapTimeEdit);
                    overlay->CheckBox("Infinite Vehicle Speed", &Data::InfiniteSpeed);
                    overlay->SliderFloat("Vehicle MaxSpeed", &Data::SpeedLimit, 10.0f, 6000.0f);
                    break;


                case 1:
                    if (overlay->Button("Main Tab ")) {
                        Tab = 0;
                    }
                    if (overlay->Button("Clean Ban files"))Data::CheatFunctions::CleanBanFiles();
                    if (overlay->Button("Randomize all Currencies"))Data::EditCurrencies = true;
                    if (overlay->Button("Menu Source "))system("start https://github.com/DumbDev69420/ScuffedGui");
                    overlay->Spacing();
                    overlay->CheckBox("Debug", &Data::Debug);
                    overlay->CheckBox("Stop Entities", &Data::StopEntities);
                    overlay->CheckBox("Fov Changer", &Data::FovChanger);
                    if (overlay->SliderFloat("Fov", &Data::Fov, 10.0f, 250.0f))Data::ForceFovChange = true;


                default:
                    break;
                }
            }

            
            
        }


        Train();

        if (ShowWaterMark)
            overlay->DrawWaterMark();



        if (Data::Debug)
            overlay->draw_text(10, 60, std::string("Entities: " + std::to_string(Data::CheatFunctions::EntityList.size())).c_str(), D2D1::ColorF(0), true);


        overlay->end_scene();


        if (GetAsyncKeyState(VK_INSERT)& 1) {
            Open = !Open;
        }

    }

    Data::Exit = true;
    Train();


    delete overlay;

    return 0;
}