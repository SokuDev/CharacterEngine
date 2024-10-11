//
// Created by PinkySmile on 09/10/24.
//

#ifndef CHARACTERENGINE_SCENES_HPP
#define CHARACTERENGINE_SCENES_HPP


#include <SokuLib.hpp>

extern int (__fastcall *og_ProfileChrSelect)(void *This);
extern int (SokuLib::Select::*og_SelectOnProcess)();
extern int (SokuLib::SelectClient::*og_SelectCLOnProcess)();
extern int (SokuLib::SelectServer::*og_SelectSVOnProcess)();

int __fastcall SelectOnProcess(SokuLib::Select *This);
int __fastcall SelectCLOnProcess(SokuLib::SelectClient *This);
int __fastcall SelectSVOnProcess(SokuLib::SelectServer *This);
int __fastcall ProfileCharacterSelect_Update(void *This);


#endif //CHARACTERENGINE_SCENES_HPP
