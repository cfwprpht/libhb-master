/*
*
*        _       _________ ______            _______  _______  _______  ______   _______  _______
*       ( \      \__   __/(  ___ \ |\     /|(  ___  )(       )(  ____ \(  ___ \ (  ____ )(  ____ \|\     /|
*       | (         ) (   | (   ) )| )   ( || (   ) || () () || (    \/| (   ) )| (    )|| (    \/| )   ( |
*       | |         | |   | (__/ / | (___) || |   | || || || || (__    | (__/ / | (____)|| (__    | | _ | |
*       | |         | |   |  __ (  |  ___  || |   | || |(_)| ||  __)   |  __ (  |     __)|  __)   | |( )| |
*       | |         | |   | (  \ \ | (   ) || |   | || |   | || (      | (  \ \ | (\ (   | (      | || || |
*       | (____/\___) (___| )___) )| )   ( || (___) || )   ( || (____/\| )___) )| ) \ \__| (____/\| () () |
*       (_______/\_______/|/ \___/ |/     \|(_______)|/     \|(_______/|/ \___/ |/   \__/(_______/(_______)
*
*
*
*/

#pragma once

#include <stdint.h>
#include <sampleutil.h>
#include <hash_map>
#include "../libLua/lua.hpp"

struct LuaValue;
struct lua_State;

namespace common {
	namespace Util {
		class LuaValue {
		public:
			typedef std::hash_map<const std::string, const LuaValue*> Table;
			enum Type { kTypeNumber, kTypeBool, kTypeString, kTypeTable, kTypeNone, };

			Type getType(void) const;

			double asNumber(void) const;

			bool asBool(void) const;

			const std::string asString(void) const;

			void setNumber(double num);

			void setBool(bool b);

			void setString(std::string str);

			const LuaValue *getField(const std::string fieldName) const;

			int getField(const std::string fieldName, const LuaValue *&lv) const;

			int getField(const char *fields[], const LuaValue *&lv) const;

			int getFieldNumber(const char *fields[], double &number) const;

			int getFieldBool(const char *field, bool &b) const;

			int getFieldNumber(const char *field, double &number) const;

			int getFieldString(const char *fields[], std::string &str) const;

			int getFieldString(const char *field, std::string &str) const;

			const LuaValue *getField(int index) const;

			static const LuaValue* parse(lua_State *L);

			~LuaValue(void);

			void _dump(int indent) const;

			void dump(void) const;

			const Table *asTable(void) const;
		private:
			LuaValue(void) {}
			Type m_type;


			union
			{
				double numberValue;
				bool   boolValue;
				std::string *stringValue;
				Table *tableValue;
			} val;


		};

		static inline bool isNumber(const common::Util::LuaValue *l)
		{
			return (l != NULL) && (l->getType() == common::Util::LuaValue::kTypeNumber);
		}
		static inline bool isTable(const common::Util::LuaValue *l)
		{
			return (l != NULL) && (l->getType() == common::Util::LuaValue::kTypeTable);
		}
		static inline bool isString(const common::Util::LuaValue *l)
		{
			return (l != NULL) && (l->getType() == common::Util::LuaValue::kTypeString);
		}

		static inline bool isBool(const common::Util::LuaValue *l)
		{
			return (l != NULL) && (l->getType() == common::Util::LuaValue::kTypeBool);
		}

		void convertStringTable(std::hash_map<std::string, std::string> &stringTable, const common::Util::LuaValue::Table *table);
	};

}