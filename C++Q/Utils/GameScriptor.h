#ifndef GAME_SCRIPTOR_H
#define GAME_SCRIPTOR_H

#include "LuaField.h"

#define  LUAH Luah::getInstance() 
#define  LUASTATE Luah::getInstance()->getState()

#define LUA_ENUM(l, val) \
	lua_pushliteral(l, #val); \
	lua_pushnumber(l, val); \
	lua_settable(l, -3)
class Role;
class Luah
{
private:
	lua_State * l; 
	bool isPackageLoaded = false;
	int currentLanguage = 0;
	int currentStage = 0;
	static const std::string ScriptFolder;
	//lua require 路径设置，cpp可以访问
	void setRequirePath(const char*);
 
	 
	Luah(const Luah&);
public:
	Luah()
	{

	};
	static Luah* getInstance()
	{
		static Luah instance;
		return &instance;
	}
	static std::string removeEnumPrefix(const std::string& str)
	{
		auto found = str.find_last_of(":");
		return str.substr(found + 1);
	}
	/*
	*执行全局，并且设置搜索文件夹，
	*返回值是stage
	*/
	static unsigned short CurrentStage;
	std::string doStage();
	void doScene();
	inline lua_State* getState() { return l; }
	inline void openLuaState() { l = luaL_newstate(); }
	inline void closeLuaState() { lua_close(l); }
	inline bool doFile(const std::string& path) { return luaL_dofile(l, path.c_str()); }
	//注意不要放在appDelegate的构造函数里面，否则安卓苹果报错
	void loadPackages();
	void registerClasses();
	void preloadResources(const char* = "Resources");
	void registerRole(const char*, const char*, Role*);
	void processTable(const char* tableName, std::function<void(LuaRef)>, const bool& = true);
	void processTableFont(const char* tableName, std::function<void(LuaRef, const FontInfo&)>, const bool& = true);
	template<typename T>
	void setValue(const char* tableName, const int& tag, const char* key, T value)
	{
		auto table = Lua::getGlobal(l, tableName);

		assert(table.isTable() && "请设置setValue tableName");

		//CCASSERT(table.len() >= tag, "setValue没有此角色标记或者表未定义");
		if (table.get(tag).isTable())
		{
			table[tag].value().set(key, value);
			flush();
		}
		else
		{
			CCLOG("[GameScriptor:setValue:%s:%d] registered role table length large than or equal to tag", tableName, tag);
		}

	}
	//专门销毁角色
	inline void setDisposal(const int& tag, const bool& val = true) { setValue(Luat_Role, tag, Luaf_IsDisposed, val); };
	inline LuaRef createTable(const int& size = 255)
	{
		LuaRef luaref;

		return luaref.createTable(l, size, size);
	};

	//注册对话框和场景等
	inline void registerRef(const char* name, cocos2d::Layer* layer)
	{
		auto ref = getGlobal(Luat_Ref);
		if (ref.isTable())
			ref.set(name, layer);
		flush();
	}
	//执行提交
	inline void flush() { lua_pcall(l, 0, 0, 0); };
	inline LuaRef getGlobal(const char* name) { return Lua::getGlobal(l, name); };
	template <typename T>
	inline void setGlobal( const char* name, const T& v) 
	{ LuaIntf::Lua::setGlobal(l, name, v); }
	LuaRef getRole(const int& tag = 0, const std::string& name = "");
	static void output(const std::string& text) { CCLOG("%s", text.c_str()); }
	//回调函数，例如 joystick joypad 滑动
	void callback(const char*, const char*);
	void callback(const char*, const char*, const LuaRef&);
	inline void callmenu(const char* className, const char* funcName, const LuaRef& params)
	{
		try
		{
			auto luaFunc = getGlobal( Luac_CallbackMenu);
			if (luaFunc.isFunction())
				luaFunc(className, funcName, params);

		}
		catch (LuaIntf::LuaException  e)
		{

#if  CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT
			log(StringUtils::format("[%s:%s]%s", className, funcName, e.what()).c_str());
#else
			//gcc ++ format 函数有问题
			log("callmenu error");
#endif
		}
	}
};
#endif
