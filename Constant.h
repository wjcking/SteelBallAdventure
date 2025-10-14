#ifndef CONSTANT_H
#define CONSTANT_H
static const float DeltaInterval = 0.01f;
//��Ļ��С
static const unsigned int ScreenWidth = 854;
static const unsigned int  ScreenHeight = 480;
//�ֱ�����
static const unsigned int  ControlPadY = 45;
//�Ⱥ�˳��
static const  short Z_Rebound = -1;
static const unsigned short Z_BACKGROUND = 1;
static const unsigned short Z_Dialog = 6;
static const unsigned short Z_MAP = 3;
static const unsigned short Z_ROLE = 5;
static const unsigned short Z_Enemy = 4;
static const unsigned short Z_RObject = 4;
static const unsigned short Z_ProjectTile = 4;
static const unsigned short Z_JoyStick = 6;
static const unsigned short Z_Overlay = 7;
static const unsigned short Z_Curtain = 8;
static const unsigned short Z_PopupLayer = 8;
static const unsigned short Z_Achieve = 8;
static const unsigned short Z_Draw = 6;
//ʵ��id
static const unsigned short ID_Player = 1;
static const unsigned short ID_Map = 1;

//�洢 
static const char User_Language[10] = "Language";
static const char User_LastStage[10] = "LastStage";

static const char User_StageEnabled[18] = "StageEnabled";
static const char User_Achieve[10] = "Achieve";
static const char User_AllowSound[15] = "AllowSound";
static const char User_AllowMusic[15] = "AllowMusic";
static const char User_IsInitialized[14] = "IsInitialized";
static const char User_Trial[8] = "Trial";

//����
static const unsigned short Lang_Chinwan = 1;
static const  unsigned short Lang_English = 2;
//lua��
static const char Luat_Ref[12] = "Ref";
static const char Luat_Role[12] = "Role";
static const char Luat_Joystick[12] = "Joystick";
static const char Luat_Joypad[12] = "Joypad";
static const char Luat_Atlas[12] = "Atlas";
static const char Luat_Achievements[15] = "Achievements";
static const char Luat_Chapters[15] = "Chapters";
static const char Luat_Checkpoint[15] = "Checkpoint";
static const char Luat_Triggers[15] = "Triggers";
static const char Luat_Sensors[15] = "Sensors";
static const char Luat_RObjectText[15] = "RObjectText";
static const char Luat_Dialogue[15] = "Dialogue";
static const char Luat_Hints[8] = "Hints";
static const char Luat_Collapse[12] = "Collapse";
static const char Luat_FrameMap[12] = "FrameMap";
static const char Luat_Curtain[12] = "Curtain";
static const char Luat_CurtainText[16] = "CurtainText";
static const char Luat_RetryDialog[16] = "RetryDialog";
static const char Luat_AchieveDialog[15] = "AchieveDialog";
static const char Luat_PromptDialog[15] = "PromptDialog";
static const char Luat_ObjectDialog[15] = "ObjectDialog";
//lua����

static const char Luac_Preload[15] = "preload";
static const char Luac_Callback[15] = "callback";
static const char Luac_CallbackMenu[15] = "callmenu";
//��������
static const char Luap_Joystick[15] = "joystick";
static const char Luap_Joypad[15] = "joypad";
//lua����
static const char Luaf_StageScene[15] = "stage";
static const char Luaf_MainScene[15] = "main";
static const char Luaf_Popup[22] = "popup";
static const char Luaf_PromptRetry[22] = "retry";
static const char Luaf_Health[3] = "hp";
static const char Luaf_HealthMax[6] = "hpMax";
static const char Luaf_Face[5] = "face";
static const char Luaf_Gid[5] = "gid";
static const char Luaf_Anchor[15] = "anchor";
static const char Luaf_Owner[15] = "owner";
static const char Luaf_UnlockAchieve[15] = "UnlockAchieve";
static const char Luaf_Triggered[10] = "trigger";
static const char Luaf_Sensed[10] = "sensed";
static const char Luaf_Sensor[10] = "sensor";
static const char Luaf_Direction[12] = "direction";
static const char Luaf_Distance[12] = "distance";
static const char Luaf_RangeType[10] = "rangeType";
static const char Luaf_Key[4] = "key";
static const char Luaf_Cmd[4] = "cmd";
static const char Luaf_Value[7] = "value";
static const char Luaf_CppRef[6] = "ref";
static const char Luaf_MapTag[7] = "mapTag";
static const char Luaf_Index[15] = "index";
static const char Luaf_RangeStart[15] = "rangeStart";
static const char Luaf_RangeEnd[15] = "rangeEnd";
static const char Luaf_Offset[8] = "offset";
static const char Luaf_AllowTouch[15] = "allowTouch";
static const char Luaf_AllowDragX[15] = "allowDragX";
static const char Luaf_AllowDragY[15] = "allowDragY";
static const char Luaf_AllowCollision[15] = "allowCollision";
static const char Luaf_DisallowCollision[25] = "disallowCollision";
static const char Luaf_AC[15] = "ac";
static const char Luaf_DC[25] = "dc";
static const char Luaf_AllowFlip[15] = "allowFlip";
static const char Luaf_AllowFollow[15] = "allowFollow";
static const char Luaf_AllowPush[15] = "allowPush";
static const char Luaf_OnCheckTile[15] = "onCheckTile";
static const char Luaf_Visible[15] = "visible";
static const char Luaf_Duration[15]= "duration";
static const char Luaf_Height[15]= "height";
static const char Luaf_DurationBack[22]= "durationBack";
static const char Luaf_Range[7]= "range";
static const char Luaf_Tag[6] = "tag";
static const char Luaf_Table[6] = "table";
static const char Luaf_Type[6] = "type";
static const char Luaf_View[6] = "view";
static const char Luaf_File[6] = "file";
static const char Luaf_Name[6] = "name";
static const char Luaf_Image[6] = "image";
static const char Luaf_Knocks[7] = "knocks";
static const char Luaf_Sound[6] = "sound";
static const char Luaf_Toast[6] = "toast";
static const char Luaf_Angle[6] = "angle";
static const char Luaf_limitedTimes[16] = "limitedTimes";
static const char Luaf_Music[6] = "music";
static const char Luaf_Frame[6] = "frame";
static const char Luaf_AllowThrough[15] = "allowThrough";
static const char Luaf_FrameIndexes[15] = "frameIndexes";
static const char Luaf_FrameSpeed[15] = "frameSpeed";
static const char Luaf_FramePrefix[15] = "framePrefix";
static const char Luaf_TouchedTimes[15] = "touchedTimes";
static const char Luaf_Plist[6] = "plist";
static const char Luaf_Rect[6] = "rect";
static const char Luaf_IsActive[12] = "isActive";
static const char Luaf_Tile[6] = "tile";
static const char Luaf_Pos[6] = "pos";
static const char Luaf_AllowGravity[14] = "allowGravity";
static const char Luaf_Gravity[8] = "gravity";
static const char Luaf_MoveStep[6] = "step";
static const char Luaf_JumpForce[16] = "jumpForce";
static const char Luaf_Text[6] = "text";
static const char Luaf_Char[6] = "char";
static const char Luaf_Switch[7] = "switch";
static const char Luaf_Choices[8] = "choices";
static const char Luaf_Count[8] = "count";
static const char Luaf_Font[6] = "font";
static const char Luaf_Bound[8] = "bound";
static const char Luaf_FontSize[5] = "size";
static const char Luaf_FontColor[7] = "color";
static const char Luaf_FontIsBold[8] = "isBold";
static const char Luaf_IsSolid[8] = "isSolid";
static const char Luaf_Delay[6] = "delay";
static const char Luaf_DelayTime[12] = "delayTime";
static const char Luaf_DelayBack[16] = "delayBack";
static const char Luaf_Stay[16] = "stay";
static const char Luaf_StayFirst[16] = "stayFirst";
static const char Luaf_InsetObjectX[7] = "insetX";
static const char Luaf_InsetObjectY[7] = "insetY";
static const char Luaf_InsetObject[15] = "insetObject";
static const char Luaf_InsetTile[15] = "insetTile";
static const char Luaf_InsetTileX[15] = "insetTileX";
static const char Luaf_InsetTileY[15] = "insetTileY";
static const char Luaf_Object[15] = "object";
static const char Luaf_IsDisposed[18] = "isDisposed";
static const char Luaf_CheckpointID[18] = "checkpointID";
static const char Luaf_MaxRounds[18] = "maxRounds";
static const char Luaf_Rate[18] = "rate";
static const char Luaf_Project[18] = "project";
static const char Luaf_Target[18] = "target";
static const char Luaf_Animate[12] = "animate";
static const char Luaf_Spring[12] = "spring";
static const char Luaf_Rebound[12] = "rebound";
static const char Luaf_DragStart[12] = "dragStart";
static const char Luaf_DragEnd[12] = "dragEnd";
static const char Luaf_ZOrder[12] = "zorder";
static const char Luaf_IsAutoStart[16] = "isAutoStart";
static const char Luaf_AutoClose[16] = "autoClose";
static const char Luaf_IsCovered[16] = "isCovered";
static const char Luaf_IsHarmful[16] = "isHarmful";
static const char Luaf_AllowBack[16] = "allowBack";
static const char Luaf_Background[16] = "background";
static const char Luaf_items[6] = "items";
static const char Luaf_Weapon[9] = "weapon";
static const char Luaf_X [2] = "x";
static const char Luaf_Y[2] = "y";
static const char Luaf_Width[11] = "width";
static const char Luaf_Interval[11] = "interval";

//��ѡ������tile���к������20��
static  const  unsigned short RangeTileMax = 25;
//reserve vector map
static const unsigned short Reserve_Role = 500;
static const unsigned short Reserve_65534 = 65535;
static const unsigned short Reserve_Delay = 500;
//popup
static const unsigned short Popup_Achieve = 3;
//ö��
//������
enum class ProjectType
{
	object,
	footboard,
	deadshot,
	trace
};
enum class CurtainType
{
	none = 0,
	black ,
	bilateral,
	updown,
	drop
};
#endif