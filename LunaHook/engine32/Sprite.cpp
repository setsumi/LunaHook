#include"Sprite.h"
 
bool Sprite_attach_function() {
  //恋と選挙とチョコレート
  auto m=GetModuleHandle(L"dirapi.dll");
  auto [minAddress, maxAddress] = Util::QueryModuleLimits(m);
  const BYTE bytes[] = {
    0x83,0xF8,0x40,
    0x74,XX,
    0x83,0xF8,0x43,
    0x74,XX,
    0x83,XX,0xFF,
    0xEB,XX,
    0x8D,0x45,0xF8,
    XX,
    XX,
    XX,
    //+20
    0xE8,XX4,
    0x89,0x45,0xF0,
    0x8D,0x45,0xF4,
    0x50,
    XX,
    0xE8,XX4
	};
  auto addr = MemDbg::findBytes(bytes, sizeof(bytes), minAddress, maxAddress);
  if(addr==0)return false; 
  if(((*(int*)(addr+22))+addr+22)!=((*(int*)(addr+35))+addr+35))return false;
  HookParam hp;
  hp.address = addr+sizeof(bytes);
  hp.offset=get_reg(regs::eax);
  hp.type = USING_STRING;  
  return NewHook(hp, "Sprite");
} 
namespace{
  bool _h1(){
    //https://vndb.org/v1714
    //[Selen]はらみこ
    auto FlashAssetx32=GetModuleHandleW(L"Flash Asset.x32");
    if(FlashAssetx32==0)return false;
    auto [s,e]=Util::QueryModuleLimits(FlashAssetx32);
    const BYTE bytes[] = {
      0x56,0x57,0x6a,0xff,
      0xff,0x75,0x08,//ebp+8
      0x53,
      0x68,0xe4,0x04,0x00,0x00,
      0xff,0x15,XX4//MultiByteToWideChar
	  };
    auto addr = MemDbg::findBytes(bytes, sizeof(bytes), s, e);
    if(addr==0)return false;
    HookParam hp;
    hp.address = addr+sizeof(bytes);//不知道从哪jump到call MultiByteToWideChar的
    hp.offset=get_stack(5);
    hp.type = USING_STRING;
    hp.filter_fun=[](LPVOID data, size_t *size, HookParam *)->bool
    {
      static int idx=0;
      return (idx++)%2;
    };
    return NewHook(hp, "Selen");
  }
}
bool Sprite::attach_function() {
  return Sprite_attach_function()|_h1();
}