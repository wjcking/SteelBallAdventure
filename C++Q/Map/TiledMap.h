
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
	//��������λ�����жϵ�ͼ�ƶ�����
	MovingDirection& getRoleDirection(const Vec2&);
	MovingDirection moveDirection = MovingDirection::stayStill;
	TiledMap* secondMap;
	std::unordered_map<int, bool> inSecondMap;

	const float mapOffsetX = 0.f;
	//�ǲ��Ǹճ����ڵ�һ�ŵ�ͼ�У���ͼѭ��ʹ��
	bool isStartOff;
	float lastRoleX;
	std::unordered_map<int, float> lastRolePositionX;

	DrawNode* drawCollision;
	TriggerSystem triggerSystem;

	//�浵

	//���ݽ�ɫÿ10���׵�ͼ�ƶ��ľ���
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
	//��ͼѭ��
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
	//������center��repeat�е���
	void setCameraY(const Vec2& pos);
	//��ͷ�ƶ�
	void setCameraCenter(Role&, const CameraView&   = CameraView::both, const bool& isAutoFocusY = false);
	//ÿһ��������x��y�ƶ�,��ע�⡿����ȡ������
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
	//����
	Vec2 getTileCoordinateByPosition(Vec2 position);
	Rect getRectByTileCoordinate(Vec2 tileCoords, const int& = 0);
	//AABB
	BoundRect* getBoundTiles(Role&, const std::string& = LayerWalls);
	BoundRect getCenterTile(const Vec2& rolePosition);
	//��
	TMXLayer& getWalls(const std::string& = LayerWalls) const;
	//lua���� wall����,Ŀǰ��������
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
	//�߽��жϷ���ios�˳�����
	uint32_t getGidAt(Vec2  tilePos, const std::string& layerName = LayerWalls);
	void removeTileRange(const Vec2&, const Vec2&);
	//**����**
	//��ô����Ͻǵ����½ǣ�ѭ�����ӵ�rect 
	Vec2 getPositionByTileCoordinate(const Point &tileCoord) const;
	//collapseʹ��
	Vec2 getPositionByTileCoordinate2(const Point &tileCoord) const;
	//����
	void debugDraw();
	void loadScript();
	//��ȫ������role manager��
	void collapse(LuaRef);
	void update(Role&);
	//һ��Ҫ������Role��Ϻ����ִ�д˷���������tag�Ḳ��ԭ�е�roleԪ��
	void registerKnocks(const LuaRef&);
	SpriteFrame* getFrameWithTile(const Vec2& pos);

	void processTileRange(const Vec2&, const Vec2&, std::function<void(const Vec2&, const short&)>);
	RangeType  getRange(const Vec2 & a, const Vec2 & b);

};

#endif