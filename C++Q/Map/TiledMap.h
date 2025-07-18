
#ifndef TMXSTAGE_H
#define TMXSTAGE_H
#include "../Utils/RectTools.h"
#include "cocos2d.h"
#include "MapProperty.h"
#include "../Triggers/TriggerSystem.h"

#include "../Utils/GameScriptor.h"
using namespace std;
using namespace cocos2d;

static const string LayerObjects = "Objects";
static const string LayerWalls = "walls";

class BoundRect;
class Role;
class Trigger;
enum class CameraView
{
	horizontal,
	vertical,
	both
};
enum class MapView
{
	panorama,
	horizontal
};
class TiledMap : public TMXTiledMap
{

private:

	Vec2 limitedSize = Vec2(ScreenSize.width, ScreenSize.height);
	Vec2 lastRolePosition = Vec2::ZERO;
	Vec2 per10msSteps = Vec2(0.f, 0.f);
	Vec2 originPosition = Vec2::ZERO;
	std::unordered_map<int, TilePropertyInfo> tileProperties;
	//根据主角位置来判断地图移动方向
	MovingDirection& getRoleDirection(const Vec2&);
	MovingDirection moveDirection = MovingDirection::stayStill;
	TiledMap* secondMap;
	std::unordered_map<int, bool> inSecondMap;

	const float mapOffsetX = 0.f;
	//是不是刚出现在第一张地图中，地图循环使用
	bool isStartOff;
	float lastRoleX;
	std::unordered_map<int, float> lastRolePositionX;

	DrawNode* drawCollision;
	TriggerSystem triggerSystem;

	//存档

	//根据角色每10毫米地图移动的距离
	Vec2 per10msDistance = Vec2::ZERO;
	inline void setLimitedSize(const float& width, const float& height)
	{
		limitedSize.x = width;
		limitedSize.y = height;
		per10msDistance = limitedSize * 0.5f;
	}
	MapView viewType = MapView::horizontal;
public:
	static int  nextTag;
	//地图循环
	inline TiledMap* getRepeatedMap() { return secondMap; };
	bool exchangeMap(Role*);
	inline bool& isRepeatedMap(const int& tagID) { return inSecondMap[tagID]; };
	TilePropertyInfo& getProperty(const int& gid);
	static TiledMap* create(const std::string& tmxFile);
	void createRepeat(const std::string& tmxFile, const bool& = true);
	inline const Vec2& getPer10Distance() const { return this->per10msDistance; }
	inline const MapView& getViewType() const { return viewType; }
	inline void setViewType(const MapView& view) { viewType = view; }
	inline Size getTotalSize() const
	{
		return  Size(getMapSize().width * getTileSize().width, getMapSize().height * getTileSize().height);
	}
	inline TriggerSystem& getTriggerSystem() { return triggerSystem; }
	TiledMap();
	TiledMap(const string& tmxFile);
	~TiledMap();
	//void collapse(const unsigned short& collapseID);
	//用来在center和repeat中调用
	void setCameraY(const Vec2& pos);
	//镜头移动
	void setCameraCenter(Role&, const CameraView&   = CameraView::both, const bool& isAutoFocusY = false);
	//每一屏，限制x，y移动,【注意】坐标取反负数
	inline void setPtc(const Vec2& pos, const Vec2 offset)
	{
		auto ptc = -getPositionByTileCoordinate2(pos);
		ptc += offset;
		setPosition(ptc);
	}
	void setCameraSlide(const Vec2& pos, const CameraView& slideStyle = CameraView::both);
	bool setCameraFrame(const bool& orientation);
	void setCameraRepeat(Role*, const float&, const bool& = true);
	bool moveCameraX(const float& step = 1.f, const float& distance = 200.f);
	//坐标
	Vec2 getTileCoordinateByPosition(Vec2 position);
	Rect getRectByTileCoordinate(Vec2 tileCoords, const int& = 0);
	//AABB
	BoundRect* getBoundTiles(Role&, const std::string& = LayerWalls);
	BoundRect getCenterTile(const Vec2& rolePosition);
	//层
	TMXLayer& getWalls(const std::string& = LayerWalls) const;
	//lua调用 wall操作,目前不能重载
	inline void setGid(const int& gid, const Vec2& coordinate) { getWalls().setTileGID(gid, coordinate); };
	inline void removeTile(const Vec2& coordinate) { getWalls().removeTileAt(coordinate); };
	void setGidRange(const int& gid, const Vec2&, const Vec2&);
	void setLayerGid(const std::string&,const int& gid, const Vec2&, const Vec2&);
	inline void setObjectGid(const int& gid, const Vec2& s, const Vec2& e)
	{
		setLayerGid("object", gid, s, e);
	}

	inline void removeObjectRange(const Vec2& s, const Vec2& e)
	{
		processTileRange(s, e, [&](const Vec2& rangeTile, const short& i) {
			getWalls("object").removeTileAt(rangeTile);
		});
	}
	inline void removeLayerRange(const std::string& name, const Vec2& s, const Vec2& e)
	{
		processTileRange(s, e, [&](const Vec2& rangeTile, const short& i) {
			getWalls(name).removeTileAt(rangeTile);
		});
	}
	//边界判断否则ios端出问题
	uint32_t getGidAt(Vec2  tilePos, const std::string& layerName = LayerWalls);
	void removeTileRange(const Vec2&, const Vec2&);
	//**结束**
	//获得从左上角到右下角，循环叠加的rect 
	Vec2 getPositionByTileCoordinate(const Point &tileCoord) const;
	//collapse使用
	Vec2 getPositionByTileCoordinate2(const Point &tileCoord) const;
	//调试
	void debugDraw();
	void loadScript();
	//在全部载入role manager后
	void collapse(LuaRef);
	void update(Role&);
	//一定要先添加Role完毕后才能执行此方法，否则tag会覆盖原有的role元素
	void registerKnocks(const LuaRef&);
	SpriteFrame* getFrameWithTile(const Vec2& pos);

	void processTileRange(const Vec2&, const Vec2&, std::function<void(const Vec2&, const short&)>);
	RangeType  getRange(const Vec2 & a, const Vec2 & b);

};

#endif