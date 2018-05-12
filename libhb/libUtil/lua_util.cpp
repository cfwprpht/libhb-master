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

#include "stdafx.h"
#include "lua_util.h"


namespace cu = common::Util;

#pragma region LuaValue

cu::LuaValue::Type cu::LuaValue::getType(void) const
{
	return m_type;
}

double cu::LuaValue::asNumber(void) const
{
	SCE_SAMPLE_UTIL_ASSERT(m_type == kTypeNumber);
	return val.numberValue;
}
bool cu::LuaValue::asBool(void) const
{
	SCE_SAMPLE_UTIL_ASSERT(m_type == kTypeBool);
	return val.boolValue;
}
const std::string cu::LuaValue::asString(void) const
{
	SCE_SAMPLE_UTIL_ASSERT(m_type == kTypeString);
	return *val.stringValue;
}

void cu::LuaValue::setNumber(double num)
{
	m_type = kTypeNumber;
	val.numberValue = num;
}

void cu::LuaValue::setBool(bool b)
{
	m_type = kTypeBool;
	val.boolValue = b;
}
void cu::LuaValue::setString(std::string str)
{
	m_type = kTypeString;
	val.stringValue = new std::string(str);
}

const cu::LuaValue *cu::LuaValue::getField(const std::string fieldName) const
{
	SCE_SAMPLE_UTIL_ASSERT(m_type == kTypeTable);
	Table::iterator it = val.tableValue->find(fieldName);
	if (it == val.tableValue->end()) {
		return NULL;
	}
	return it->second;
}

int cu::LuaValue::getField(const std::string fieldName, const LuaValue *&lv) const
{
	if (m_type != kTypeTable) {
		return -1;
	}
	Table::iterator it = val.tableValue->find(fieldName);
	if (it == val.tableValue->end()) {
		return -1;
	}
	lv = it->second;
	return SCE_OK;
}

int cu::LuaValue::getField(const char *fields[], const LuaValue *&lv) const
{
	if (m_type != kTypeTable) {
		return -1;
	}
	if (fields == NULL) {
		return -1;
	}
	if (fields[0] == NULL) {
		return -1;
	}
	const LuaValue *lv2 = NULL;
	int ret = getField(fields[0], lv2);
	if (ret != SCE_OK) {
		return ret;
	}
	if (fields[1] == NULL) {
		lv = lv2;
		return SCE_OK;
	}
	else {
		return getField(&fields[1], lv);
	}
}

int cu::LuaValue::getFieldNumber(const char *fields[], double &number) const
{
	const LuaValue *lv = NULL;
	int ret = getField(fields, lv);
	if (ret != SCE_OK) {
		return ret;
	}
	if (lv->m_type != kTypeNumber) {
		return -1;
	}
	number = lv->asNumber();
	return SCE_OK;
}

int cu::LuaValue::getFieldBool(const char *field, bool &b) const
{
	const char* fs[] = { field, NULL };
	const LuaValue *lv = NULL;
	int ret = getField(fs, lv);
	if (ret != SCE_OK) {
		return ret;
	}
	if (lv->m_type != kTypeBool) {
		return -1;
	}
	b = lv->asBool();
	return SCE_OK;
}

int cu::LuaValue::getFieldNumber(const char *field, double &number) const
{
	const char* fs[] = { field, NULL };
	return getFieldNumber(fs, number);
}

int cu::LuaValue::getFieldString(const char *fields[], std::string &str) const
{
	const LuaValue *lv = NULL;
	int ret = getField(fields, lv);
	if (ret != SCE_OK) {
		return ret;
	}
	if (lv->m_type != kTypeString) {
		return -1;
	}
	str = lv->asString();
	return SCE_OK;
}

int cu::LuaValue::getFieldString(const char *field, std::string &str) const
{
	const char* fs[] = { field, NULL };
	return getFieldString(fs, str);
}

const cu::LuaValue *cu::LuaValue::getField(int index) const
{
	SCE_SAMPLE_UTIL_ASSERT(m_type == kTypeTable);
	char buf[sizeof(int) * 8 + 1];
	snprintf(buf, sizeof(int) * 8 + 1, "%d", index);
	std::string fieldName(buf);
	Table::iterator it = val.tableValue->find(fieldName);
	if (it == val.tableValue->end()) {
		return NULL;
	}
	return it->second;
}

const cu::LuaValue* cu::LuaValue::parse(lua_State *L)
{
	LuaValue* ret = NULL;
	int top = lua_gettop(L);
	switch (lua_type(L, -1))
	{
	case LUA_TNUMBER:
	{
		double v = lua_tonumber(L, lua_gettop(L));
		ret = new LuaValue;
		ret->setNumber(v);
		break;
	}
	case LUA_TBOOLEAN:
	{
		bool v = lua_toboolean(L, lua_gettop(L)) != 0;
		ret = new LuaValue;
		ret->setBool(v);
		break;
	}
	case LUA_TSTRING:
	{
		std::string v = lua_tolstring(L, lua_gettop(L), NULL);
		ret = new LuaValue;
		ret->setString(v);
		break;
	}
	case LUA_TTABLE:
	{
		ret = new LuaValue;
		ret->m_type = kTypeTable;
		ret->val.tableValue = new Table;

		lua_pushnil(L);
		while (lua_next(L, top) != 0) {
			std::string name = lua_tostring(L, -2);
			bool isNumber = lua_isnumber(L, -2);

			const LuaValue* child = parse(L);
			ret->val.tableValue->insert(std::pair<std::string, const LuaValue* >(name, child));

			lua_pop(L, 1);

			if (isNumber) { // is this needed?
				double d = lua_tonumber(L, -1);
				lua_pop(L, 1);
				lua_pushnumber(L, d);
			}
		}
		lua_settop(L, top);
		break;

	}
	case LUA_TNIL:
	{
		break;
	}
	default:
	{
		int type = lua_type(L, -1);
		printf("type=%d\n", type);
		SCE_SAMPLE_UTIL_ASSERT(0);
	}
	}
	lua_settop(L, top);
	return ret;
}

cu::LuaValue::~LuaValue(void)
{
	switch (m_type)
	{
	case kTypeNumber:
	{
		break;
	}
	case kTypeBool:
	{
		break;
	}
	case kTypeString:
	{
		delete val.stringValue;
		val.stringValue = NULL;
		break;
	}
	case kTypeTable:
	{
		Table &t = *val.tableValue;
		for (Table::iterator it = t.begin(); it != t.end(); it++) {
			const LuaValue *l = it->second;
			delete l;
		}
		delete val.tableValue;
		val.tableValue = NULL;
		break;
	}
	default:
		SCE_SAMPLE_UTIL_ASSERT(0);
	}
	m_type = kTypeNone;
}

void cu::LuaValue::_dump(int indent) const
{
	switch (m_type)
	{
	case kTypeNumber:
	{
		printf("%f\n", val.numberValue);
		break;
	}
	case kTypeBool:
	{
		printf(val.boolValue ? "TRUE" : "FALSE");
		break;
	}
	case kTypeString:
	{
#if 0
		printf(val.stringValue->c_str());
#endif
		break;
	}
	case kTypeTable:
	{
		printf("-- table --\n");
		Table &t = *val.tableValue;
		for (Table::iterator it = t.begin(); it != t.end(); it++) {
			const std::string field = it->first;
			const LuaValue *l = it->second;
			printf("%s : ", field.c_str());
			l->_dump(indent + 1);
		}
		break;
	}
	default:
		SCE_SAMPLE_UTIL_ASSERT(0);
	}
}

void cu::LuaValue::dump(void) const
{
	_dump(0);
}

const cu::LuaValue::Table *cu::LuaValue::asTable(void) const
{
	if (m_type != kTypeTable) {
		return NULL;
	}
	return val.tableValue;
}


#pragma endregion // LuaValue

void cu::convertStringTable(std::hash_map<std::string, std::string> &stringTable, const common::Util::LuaValue::Table *table)
{
	stringTable.clear();
	common::Util::LuaValue::Table::const_iterator it;
	for (it = table->begin(); it != table->end(); it++) {
		std::string name = it->first;
		const common::Util::LuaValue *lv = it->second;
		SCE_SAMPLE_UTIL_ASSERT(lv->getType() == common::Util::LuaValue::kTypeString);//TODO:
		stringTable[name] = lv->asString();
	}
}