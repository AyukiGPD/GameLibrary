//=============================================================================
//!
//!	@file	KeyInput.h
//!	@brief	キーデータ
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//!	取得モード
enum class KEY_MODE
{
	BUTTON,
	TRIGGER,
	RELEASE,
};

//!	キーコード
enum class KEYCODE : u8
{
	PAD_LBUTTON				= VK_LBUTTON,				//!< 0x01	マウス左ボタン
	PAD_RBUTTON				= VK_RBUTTON,				//!< 0x02	マウス右ボタン
	PAD_CANCEL				= VK_CANCEL,				//!< 0x03	Ctrl - Break
	PAD_MBUTTON				= VK_MBUTTON,				//!< 0x04	マウス中央ボタン

	#if(_WIN32_WINNT >= 0x0500)
	PAD_XBUTTON1			= VK_XBUTTON1,				//!< 0x05    /* NOT contiguous with L & RBUTTON */
	PAD_XBUTTON2			= VK_XBUTTON2,				//!< 0x06    /* NOT contiguous with L & RBUTTON */
	#endif /* _WIN32_WINNT >= 0x0500 */

	//0x07 : unassigned

	PAD_BACK				= VK_BACK,					//!< 0x08	Backspace
	PAD_TAB					= VK_TAB,					//!< 0x09	Tab

	//0x0A - 0x0B : reserved

	PAD_CLEAR				= VK_CLEAR,					//!< 0x0C	NumLock を外した状態のテンキー5
	PAD_ENTER				= VK_RETURN,				//!< 0x0D	Enter

	PAD_SHIFT				= VK_SHIFT,					//!< 0x10	Shift
	PAD_CONTROL				= VK_CONTROL,				//!< 0x11	Ctrl
	PAD_MENU				= VK_MENU,					//!< 0x12	Alt
	PAD_PAUSE				= VK_PAUSE,					//!< 0x13	Pause
	PAD_CAPITAL				= VK_CAPITAL,				//!< 0x14	Caps Lock

	PAD_KANA				= VK_KANA,					//!< 0x15
	PAD_HANGEUL				= VK_HANGEUL,				//!< 0x15
	PAD_HANGUL				= VK_HANGUL,				//!< 0x15
	PAD_JUNJA				= VK_JUNJA,					//!< 0x17
	PAD_FINAL				= VK_FINAL,					//!< 0x18
	PAD_HANJA				= VK_HANJA,					//!< 0x19
	PAD_KANJI				= VK_KANJI,					//!< 0x19

	PAD_ESCAPE				= VK_ESCAPE,				//!< 0x1B	Esc

	PAD_CONVERT				= VK_CONVERT,				//!< 0x1C
	PAD_NONCONVERT			= VK_NONCONVERT,			//!< 0x1D
	PAD_ACCEPT				= VK_ACCEPT,				//!< 0x1E
	PAD_MODECHANGE			= VK_MODECHANGE,			//!< 0x1F

	PAD_SPACE				= VK_SPACE,					//!< 0x20	Spacebar
	PAD_PRIOR				= VK_PRIOR,					//!< 0x21	Page Up
	PAD_NEXT				= VK_NEXT,					//!< 0x22	Page Down
	PAD_END					= VK_END,					//!< 0x23	End
	PAD_HOME				= VK_HOME,					//!< 0x24	Home
	PAD_LEFT				= VK_LEFT,					//!< 0x25	左方向キー
	PAD_UP					= VK_UP,					//!< 0x26	上方向キー
	PAD_RIGHT				= VK_RIGHT,					//!< 0x27	右方向キー
	PAD_DOWN				= VK_DOWN,					//!< 0x28	下方向キー
	PAD_SELECT				= VK_SELECT,				//!< 0x29
	PAD_PRINT				= VK_PRINT,					//!< 0x2A
	PAD_EXECUTE				= VK_EXECUTE,				//!< 0x2B
	PAD_SNAPSHOT			= VK_SNAPSHOT,				//!< 0x2C	PringScreen
	PAD_INSERT				= VK_INSERT,				//!< 0x2D	Insert
	PAD_DELETE				= VK_DELETE,				//!< 0x2E	Delete
	PAD_HELP				= VK_HELP,					//!< 0x2F

	PAD_0					= '0',						//!< 0
	PAD_1					= '1',						//!< 1
	PAD_2					= '2',						//!< 2
	PAD_3					= '3',						//!< 3
	PAD_4					= '4',						//!< 4
	PAD_5					= '5',						//!< 5
	PAD_6					= '6',						//!< 6
	PAD_7					= '7',						//!< 7
	PAD_8					= '8',						//!< 8
	PAD_9					= '9',						//!< 9
	PAD_A					= 'A',						//!< A
	PAD_B					= 'B',						//!< B
	PAD_C					= 'C',						//!< C
	PAD_D					= 'D',						//!< D
	PAD_E					= 'E',						//!< E
	PAD_F					= 'F',						//!< F
	PAD_G					= 'G',						//!< G
	PAD_H					= 'H',						//!< H
	PAD_I					= 'I',						//!< I
	PAD_J					= 'J',						//!< J
	PAD_K					= 'K',						//!< K
	PAD_L					= 'L',						//!< L
	PAD_M					= 'M',						//!< M
	PAD_N					= 'N',						//!< N
	PAD_O					= 'O',						//!< O
	PAD_P					= 'P',						//!< P
	PAD_Q					= 'Q',						//!< Q
	PAD_R					= 'R',						//!< R
	PAD_S					= 'S',						//!< S
	PAD_T					= 'T',						//!< T
	PAD_U					= 'U',						//!< U
	PAD_V					= 'V',						//!< V
	PAD_W					= 'W',						//!< W
	PAD_X					= 'X',						//!< X
	PAD_Y					= 'Y',						//!< Y
	PAD_Z					= 'Z',						//!< Z

	PAD_LWIN				= VK_LWIN,					//!< 0x5B	左 Windows キー
	PAD_RWIN				= VK_RWIN,					//!< 0x5C	右 Windows キー
	PAD_APPS				= VK_APPS,					//!< 0x5D	アプリケーションキー
		
	//0x5E : reserved

	PAD_SLEEP				= VK_SLEEP,					//!< 0x5F

	PAD_NUMPAD0				= VK_NUMPAD0,				//!< 0x60	NumLock 0
	PAD_NUMPAD1				= VK_NUMPAD1,				//!< 0x61	NumLock 1
	PAD_NUMPAD2				= VK_NUMPAD2,				//!< 0x62	NumLock 2
	PAD_NUMPAD3				= VK_NUMPAD3,				//!< 0x63	NumLock 3
	PAD_NUMPAD4				= VK_NUMPAD4,				//!< 0x64	NumLock 4
	PAD_NUMPAD5				= VK_NUMPAD5,				//!< 0x65	NumLock 5
	PAD_NUMPAD6				= VK_NUMPAD6,				//!< 0x66	NumLock 6
	PAD_NUMPAD7				= VK_NUMPAD7,				//!< 0x67	NumLock 7
	PAD_NUMPAD8				= VK_NUMPAD8,				//!< 0x68	NumLock 8
	PAD_NUMPAD9				= VK_NUMPAD9,				//!< 0x69	NumLock 9
	PAD_MULTIPLY			= VK_MULTIPLY,				//!< 0x6A	テンキー *
	PAD_ADD					= VK_ADD,					//!< 0x6B	テンキー +
	PAD_SEPARATOR			= VK_SEPARATOR,				//!< 0x6C
	PAD_SUBTRACT			= VK_SUBTRACT,				//!< 0x6D	テンキー -
	PAD_DECIMAL				= VK_DECIMAL,				//!< 0x6E	テンキー .
	PAD_DIVIDE				= VK_DIVIDE,				//!< 0x6F	テンキー /
	PAD_F1					= VK_F1	,					//!< 0x70	F1
	PAD_F2					= VK_F2	,					//!< 0x71	F2
	PAD_F3					= VK_F3	,					//!< 0x72	F3
	PAD_F4					= VK_F4	,					//!< 0x73	F4
	PAD_F5					= VK_F5	,					//!< 0x74	F5
	PAD_F6					= VK_F6	,					//!< 0x75	F6
	PAD_F7					= VK_F7	,					//!< 0x76	F7
	PAD_F8					= VK_F8	,					//!< 0x77	F8
	PAD_F9					= VK_F9	,					//!< 0x78	F9
	PAD_F10					= VK_F10,					//!< 0x79	F10
	PAD_F11					= VK_F11,					//!< 0x7A	F11
	PAD_F12					= VK_F12,					//!< 0x7B	F12
	PAD_F13					= VK_F13,					//!< 0x7C	F13
	PAD_F14					= VK_F14,					//!< 0x7D	F14
	PAD_F15					= VK_F15,					//!< 0x7E	F15
	PAD_F16					= VK_F16,					//!< 0x7F	F16
	PAD_F17					= VK_F17,					//!< 0x80	F17
	PAD_F18					= VK_F18,					//!< 0x81	F18
	PAD_F19					= VK_F19,					//!< 0x82	F19
	PAD_F20					= VK_F20,					//!< 0x83	F20
	PAD_F21					= VK_F21,					//!< 0x84	F21
	PAD_F22					= VK_F22,					//!< 0x85	F22
	PAD_F23					= VK_F23,					//!< 0x86	F23
	PAD_F24					= VK_F24,					//!< 0x87	F24

	//0x88 - 0x8F : unassigned


	PAD_NUMLOCK				= VK_NUMLOCK,				//!< 0x90	Num Lock
	PAD_SCROLL				= VK_SCROLL,				//!< 0x91	Scroll

	//	NEC PC-9800 kbd definitions
	PAD_OEM_NEC_EQUAL		= VK_OEM_NEC_EQUAL,			//!< 0x92	'=' key on numpad

	//Fujitsu/OASYS kbd definitions
	PAD_OEM_FJ_JISHO		= VK_OEM_FJ_JISHO,			//!< 0x92   // 'Dictionary' key
	PAD_OEM_FJ_MASSHOU		= VK_OEM_FJ_MASSHOU,		//!< 0x93   // 'Unregister word' key
	PAD_OEM_FJ_TOUROKU		= VK_OEM_FJ_TOUROKU,		//!< 0x94   // 'Register word' key
	PAD_OEM_FJ_LOYA			= VK_OEM_FJ_LOYA,			//!< 0x95   // 'Left OYAYUBI' key
	PAD_OEM_FJ_ROYA			= VK_OEM_FJ_ROYA,			//!< 0x96   // 'Right OYAYUBI' key

	PAD_LSHIFT				= VK_LSHIFT,				//!< 0xA0	Windows NT : 左Shiftキー
	PAD_RSHIFT				= VK_RSHIFT,				//!< 0xA1	Windows NT : 右Shiftキー
	PAD_LCONTROL			= VK_LCONTROL,				//!< 0xA2	Windows NT : 左Ctrlキー
	PAD_RCONTROL			= VK_RCONTROL,				//!< 0xA3	Windows NT : 右Ctrlキー
	PAD_LMENU				= VK_LMENU,					//!< 0xA4	Windows NT : 左Altキー
	PAD_RMENU				= VK_RMENU,					//!< 0xA5	Windows NT : 右Altキー

	#if(_WIN32_WINNT >= 0x0500)
	PAD_BROWSER_BACK        = VK_BROWSER_BACK,			//!< 0xA6
	PAD_BROWSER_FORWARD     = VK_BROWSER_FORWARD,		//!< 0xA7
	PAD_BROWSER_REFRESH     = VK_BROWSER_REFRESH,		//!< 0xA8
	PAD_BROWSER_STOP        = VK_BROWSER_STOP,			//!< 0xA9
	PAD_BROWSER_SEARCH      = VK_BROWSER_SEARCH,		//!< 0xAA
	PAD_BROWSER_FAVORITES   = VK_BROWSER_FAVORITES,		//!< 0xAB
	PAD_BROWSER_HOME        = VK_BROWSER_HOME,			//!< 0xAC

	PAD_VOLUME_MUTE         = VK_VOLUME_MUTE,			//!< 0xAD
	PAD_VOLUME_DOWN         = VK_VOLUME_DOWN,			//!< 0xAE
	PAD_VOLUME_UP           = VK_VOLUME_UP,				//!< 0xAF
	PAD_MEDIA_NEXT_TRACK    = VK_MEDIA_NEXT_TRACK,		//!< 0xB0
	PAD_MEDIA_PREV_TRACK    = VK_MEDIA_PREV_TRACK,		//!< 0xB1
	PAD_MEDIA_STOP          = VK_MEDIA_STOP,			//!< 0xB2
	PAD_MEDIA_PLAY_PAUSE    = VK_MEDIA_PLAY_PAUSE,		//!< 0xB3
	PAD_LAUNCH_MAIL         = VK_LAUNCH_MAIL,			//!< 0xB4
	PAD_LAUNCH_MEDIA_SELECT = VK_LAUNCH_MEDIA_SELECT,	//!< 0xB5
	PAD_LAUNCH_APP1         = VK_LAUNCH_APP1,			//!< 0xB6
	PAD_LAUNCH_APP2         = VK_LAUNCH_APP2,			//!< 0xB7
	#endif /* _WIN32_WINNT >= 0x0500 */

		//0xB8 - 0xB9 : reserved

	PAD_OEM_1				= VK_OEM_1,					//!< 0xBA   ';:' for US
	PAD_OEM_PLUS			= VK_OEM_PLUS,				//!< 0xBB   '+' any country
	PAD_OEM_COMMA			= VK_OEM_COMMA,				//!< 0xBC   ',' any country
	PAD_OEM_MINUS			= VK_OEM_MINUS,				//!< 0xBD   '-' any country
	PAD_OEM_PERIOD			= VK_OEM_PERIOD,			//!< 0xBE   '.' any country
	PAD_OEM_2				= VK_OEM_2,					//!< 0xBF   '/?' for US
	PAD_OEM_3				= VK_OEM_3,					//!< 0xC0   '`~' for US

	//0xC1 - 0xD7 : reserved

	//0xD8 - 0xDA : unassigned

	PAD_OEM_4				= VK_OEM_4,					//!< 0xDB  '[{' for US
	PAD_OEM_5				= VK_OEM_5,					//!< 0xDC  '\|' for US
	PAD_OEM_6				= VK_OEM_6,					//!< 0xDD  ']}' for US
	PAD_OEM_7				= VK_OEM_7,					//!< 0xDE  ''"' for US
	PAD_OEM_8				= VK_OEM_8,					//!< 0xDF

	//0xE0 : reserved

	PAD_OEM_AX				= VK_OEM_AX,				//!< 0xE1   'AX' key on Japanese AX kbd
	PAD_OEM_102				= VK_OEM_102,				//!< 0xE2   "<>" or "\|" on RT 102-key kbd.
	PAD_ICO_HELP			= VK_ICO_HELP,				//!< 0xE3   Help key on ICO
	PAD_ICO_00				= VK_ICO_00,				//!< 0xE4   00 key on ICO

	#if(WINVER >= 0x0400)
	PAD_PROCESSKEY			= VK_PROCESSKEY,			//!< 0xE5
	#endif /* WINVER >= 0x0400 */

	PAD_ICO_CLEAR	= VK_ICO_CLEAR,						//!< 0xE6

	#if(_WIN32_WINNT >= 0x0500)
	PAD_PACKET				= VK_PACKET,				//!< 0xE7
	#endif /* _WIN32_WINNT >= 0x0500 */

	//	0xE8 : unassigned

	PAD_OEM_RESET			= VK_OEM_RESET,				//!< 0xE9
	PAD_OEM_JUMP			= VK_OEM_JUMP,				//!< 0xEA
	PAD_OEM_PA1				= VK_OEM_PA1,				//!< 0xEB
	PAD_OEM_PA2				= VK_OEM_PA2,				//!< 0xEC
	PAD_OEM_PA3				= VK_OEM_PA3,				//!< 0xED
	PAD_OEM_WSCTRL			= VK_OEM_WSCTRL,			//!< 0xEE
	PAD_OEM_CUSEL			= VK_OEM_CUSEL,				//!< 0xEF
	PAD_OEM_ATTN			= VK_OEM_ATTN,				//!< 0xF0
	PAD_OEM_FINISH			= VK_OEM_FINISH,			//!< 0xF1
	PAD_OEM_COPY			= VK_OEM_COPY,				//!< 0xF2
	PAD_OEM_AUTO			= VK_OEM_AUTO,				//!< 0xF3
	PAD_OEM_ENLW			= VK_OEM_ENLW,				//!< 0xF4
	PAD_OEM_BACKTAB			= VK_OEM_BACKTAB,			//!< 0xF5

	PAD_ATTN				= VK_ATTN,					//!< 0xF6
	PAD_CRSEL				= VK_CRSEL,					//!< 0xF7
	PAD_EXSEL				= VK_EXSEL,					//!< 0xF8
	PAD_EREOF				= VK_EREOF,					//!< 0xF9
	PAD_PLAY				= VK_PLAY,					//!< 0xFA
	PAD_ZOOM				= VK_ZOOM,					//!< 0xFB
	PAD_NONAME				= VK_NONAME,				//!< 0xFC
	PAD_PA1					= VK_PA1,					//!< 0xFD
	PAD_OEM_CLEAR			= VK_OEM_CLEAR,				//!< 0xFE

	//	0xFF : reserved
};

class KeyInputManager : public ManagerBase
{
public:
	//!	コンストラクタ
	KeyInputManager();
	//!	デストラクタ
	virtual ~KeyInputManager();

	//!	更新
	void Update();

	//!	キー取得
	//!	@param	[in]	pad		キーコード
	//!	@param	[in]	mode	取得モード
	bool GetButtons(KEYCODE pad, KEY_MODE mode) const
	{
		switch(mode)
		{
		case KEY_MODE::BUTTON:
			return GetButtons(pad);
			break;
		case KEY_MODE::TRIGGER:
			return GetTriggers(pad);
			break;
		case KEY_MODE::RELEASE:
			return GetReleases(pad);
			break;
		}
	}

	//!	生キー取得
	//!	@param	[in]	pad	キーコード
	bool GetButtons(KEYCODE pad) const
	{
		u32 padVal = (u32)pad;
		u32 index = padVal == 0 ? 0 : (padVal / 32);
		u32 shift = padVal - (index * 32);
		return (_buttons[index] & (1 << shift)) != 0;
	}
	//!	トリガー
	//!	@param	[in]	pad	キーコード
	bool GetTriggers(KEYCODE pad) const
	{
		u32 padVal = (u32)pad;
		u32 index = padVal == 0 ? 0 : (padVal / 32);
		u32 shift = padVal - (index * 32);
		return (_triggers[index] & (1 << shift)) != 0;
	}
	//!	リリース
	//!	@param	[in]	pad	キーコード
	bool GetReleases(KEYCODE pad) const
	{
		u32 padVal = (u32)pad;
		u32 index = padVal == 0 ? 0 : (padVal / 32);
		u32 shift = padVal - (index * 32);
		return (_releases[index] & (1 << shift)) != 0;
	}

private:
	u32		_buttons[8];		//!< 生キーデータ
	u32		_triggers[8];		//!< トリガーキーデータ
	u32		_releases[8];		//!< リリースキーデータ

public:
};


MANAGER_INTERFACE(KeyInputManager, KeyInput);


