--测试状态机在里面
require("global")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local NpcStates = require("npcstates")
local Telegram = require("telegram")
local CommonStates = require("commonstates")
local State7= require("state7")
PopupInfo = nil
Resources =
{  
{ music = Music.RaymanNight }, 
}
Background =
{
  {file= Folder.background.."night.png",},
}
-- 地图
Atlas =
{
  {file = "map/map7.tmx" , view =  MapView.horizontal,  }  
}
Triggers =
{
  { type=0,name="trap",}
}

--对话范围 cpp下标 0 开始
Segment =
{
  { rangeStart = 0,  rangeEnd= 9, question=1, },
}


--如果lua中上一次的collapse存在，务必重置后才能使用否则会出现range越界,定义变量不赋值则可以清空
Collapse =
{

}
CommandPad = 
{
  jump = "jump",
  drop = "drop",
  open = "open",
}
Joystick =
{
  type = 2,
  controlTag = 1,
}
Joypad =
{
  { file = "control/jump.png",pos = Vec2(ScreenWidth - 70, 45), name = CommandPad.jump },
  { file = "control/open.png",pos = Vec2(ScreenWidth - 70 * 2.5, 45), name = CommandPad.open, visible= false, }, 
}

RoleIndex =
{
  zzi=2,
  brick1 = 3,
  brick2 = 4,
  brick3 = 5,
  char1= 6,
  char2=7,
  char3=8,
  char4=9,
}
local knockChar =
{ 
  {type = KnockAction.popup,direction=CollisionDirection.atBottom,limitedTimes=-1,},
}
Role = 
{
  { tag = 1,type = RoleType.player,frame = "player1.png",pos = Vec2(13,12),face=MovingDirection.toLeft,},
  { tag = 2,type = RoleType.npc,frame = "zzi1.png",pos = Vec2(2,8), offset=Vec2(0,18), allowGravity=false,wrongNumber=0,},
  { tag = 3,type = RoleType.robject,tile = Tile.grayBrick,pos = Vec2(1,9),onCheckTile=false,},
  { tag = 4,type = RoleType.robject,tile = Tile.grayBrick,pos = Vec2(2,9),onCheckTile=false,},
  { tag = 5,type = RoleType.robject,tile = Tile.grayBrick,pos = Vec2(3,9),onCheckTile=false,},

  { tag = 6,type = RoleType.robject,tile = Tile.charBlock,pos = Vec2(6,10) ,knocks=knockChar,offset=Vec2(0,10), onCheckTile=false, char={text='A',font=Font.char,},},
  { tag = 7,type = RoleType.robject,tile = Tile.charBlock,pos = Vec2(8,10) ,knocks=knockChar,offset=Vec2(0,10), onCheckTile=false,char={text='B',font=Font.char},},
  { tag = 8,type = RoleType.robject,tile = Tile.charBlock,pos = Vec2(10,10),knocks=knockChar,offset=Vec2(0,10), onCheckTile=false,char={text='C',font=Font.char},},
  { tag = 9,type = RoleType.robject,tile = Tile.charBlock,pos = Vec2(12,10),knocks=knockChar,offset=Vec2(0,10), onCheckTile=false,char={text='D',font=Font.char},},
}

--回调事件，funcName函数名
callback = function(className,funcName,params) 
  --全局回调函数
  local isTerminated = globalCallFunc(className,funcName,params)

  if (isTerminated) then
    return
  end
  if (className == "gun" and funcName =="shoot") then
    RoleModel.addProjectObject(params.ref)
  end
  --对话,放在stage里面否则会点击好几次
  if (className == "stage") then
    DialogueSystem.proceed()
  end
-- 冻结以后什么都不能做
  if ( isFreezed()) then
    return
  end
  if (Role[1].ref:getBodyStatus() == BodyStatus.dead) then
    Role[1].ref:freeze()
  end
--方向杆
  if (className == "joystick") then
    Role[1].ref:control(Joystick.ref:getDirection(),Joystick.ref:getAngle())

  end
--控制器按钮
  if (className == "joypad" and funcName == "joypad") then
    Role[1].ref.onJump = Joystick.ref:getTouchedPad(1)
  end
end
preload = function()
  --根据语言载入对话,cpp动态写入语言，必须在函数内执行
  require("dialog".."7-"..Language)
  PopupInfo = require("popup"..Language)
  --预先载入公共事件或属性，比如Role里面的事件
  preloadStage()

  local iter = 1
  Role[iter].fsm = Fsm:new(Role[iter],State7.Player.Routine,nil) iter = iter + 1

  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1


  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1


  Role[iter].fsm = Fsm:new(Role[iter],nil,State7.RObject.BlockChoice) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State7.RObject.BlockChoice) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State7.RObject.BlockChoice) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State7.RObject.BlockChoice) iter = iter + 1

  --触发器
  iter = 1 
  Triggers[1].fsm = Fsm:new(Triggers[iter],nil,State7.Triggers.Trap) iter = iter + 1
end
-- cpp初始化调用，
initiate = function()
 -- Ads.showBanner(Banner)
   Ref.stage:loadCurtain({type = CurtainType.bilateral, image={"background/curtainleft.png","background/curtainright.png"},  delay = 1, zorder = ZOrder.dropCurtain})
  if (Checkpoint.id == 0) then
    Segment[1].fsm:changeState(State7.Dialogue.Dialog)
  else
    Role[1].ref:spawn(Vec2(13,12),Vec2.ZERO)
    Role[RoleIndex.zzi].fsm:changeState(State7.Npc.ZziQuestion)
  end
  Resh.playMusic(Music.RaymanNight)
  globalInit()
end
--循环调用
update = function()
  
  if(checkRetry(2.5)) then
    updateGlobal()
  end
end