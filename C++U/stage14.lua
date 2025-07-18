--测试状态机在里面
require("global")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local NpcStates = require("npcstates")
local Telegram = require("telegram")
local CommonStates = require("commonstates")
local State14= require("state14")
PopupInfo = nil
Resources =
{  
  { music = Music.AdventureTime }, 
  {plist = Folder.texture.."auto"},--不要加扩展名.png
}
Background =
{
  {file= Folder.background.."dusk.png",},
}
-- 地图
Atlas =
{
  {file = "map/map14.tmx" , view =  MapView.horizontal,   }   --image=Folder.background.."stage14.png",
}
Triggers =
{

}

--对话范围 cpp下标 0 开始
Segment =
{
  { rangeStart = 0,  rangeEnd= 5, question=1, },
}

--如果lua中上一次的collapse存在，务必重置后才能使用否则会出现range越界,定义变量不赋值则可以清空
Collapse =
{

}
CommandPad = 
{
  jump = "jump", 
  fire="fire",
}
Joystick =
{
  type = 2,
  controlTag = 1,
  visible=false,
}
Joypad =
{
  { file = "control/jump.png",pos = Vec2(ScreenWidth - 70, 45), name = CommandPad.jump },
}

RoleIndex =
{
  wangba=2,
  pickup=3,
  roadSign=10,
  sun=9,
  cloud1 = 4,
  cloud2 = 5,
  cloud3 = 6,
  cloud4 = 7,
  cloud5 = 8,
  cloudHints = 11,
}

local step=Vec2(0.2,0.2)
local stepCloud=Vec2(0.2,0.2)
    local   gunDeadshot = { count=1,  sound=Folder.sound.."poogun.wav",rate=1, frame="poo.png",project={type=ProjectType.deadshot,step=Vec2(2,2),allowGravity=false,target=RoleIndex.sun}}
Role = 
{
  { tag = 1,type = RoleType.player,frame = "rage1.png",pos = Vec2(0,9),face=MovingDirection.toRight, frameIndexes=FrameTemplate.playerRage,step=step,},
  { tag = 2,type = RoleType.npc,frame ="wb1.png", frameIndexes=FrameTemplate.wb,pos = Vec2(6,7), offset=Vec2(0,-15),allowGravity=false, onCheckTile=false,  face=MovingDirection.toLeft, zorder=ZOrder.cover, step=step, allowFlip=false, isHarmful=false,hp=300,hpMax=300, weapon=gunDeadshot},  
  { tag = 3,type = RoleType.robject,frame = "pickup1.png",pos = Vec2(7,9), offset=Vec2(0,0),  face=MovingDirection.toRight, frameIndexes=FrameTemplate.pickup, allowGravity=true,zorder=ZOrder.cover,onCheckTile=true, isSolid=false,isHarmful=false,step=step,isExcepted=true,},

  { tag = 4,type = RoleType.robject,frame = "cloud11.png",pos = Vec2(18,4), offset=Vec2(0,0),  direction=MovingDirection.toLeft,  allowGravity=false,zorder=ZOrder.cover,onCheckTile=true, isSolid=false,isHarmful=false,step=stepCloud, distance=Vec2(ScreenWidth*1.2,ScreenHeight), allowFlip=false, isExcepted=true,},
  { tag = 5,type = RoleType.robject,frame = "cloud21.png",pos = Vec2(22,5), offset=Vec2(0,0),  direction=MovingDirection.toLeft,   allowGravity=false,zorder=ZOrder.cover,onCheckTile=true, isSolid=false,isHarmful=false,step=stepCloud,distance=Vec2(ScreenWidth*1.6,ScreenHeight), allowFlip=false, zorder=ZOrder.overlay,isExcepted=true,},
  { tag = 6,type = RoleType.robject,frame = "cloud31.png",pos = Vec2(26,4), offset=Vec2(0,0),  direction=MovingDirection.toLeft,  allowGravity=false,zorder=ZOrder.cover,onCheckTile=true, isSolid=false,isHarmful=false,step=stepCloud,distance=Vec2(ScreenWidth*1.8,ScreenHeight), allowFlip=false,isExcepted=true,},
  { tag = 7,type = RoleType.robject,frame = "cloud11.png",pos = Vec2(30,5), offset=Vec2(0,0),  direction=MovingDirection.toLeft,  allowGravity=false,zorder=ZOrder.cover,onCheckTile=true, isSolid=false,isHarmful=false,step=stepCloud,distance=Vec2(ScreenWidth*2,ScreenHeight), allowFlip=false,zorder=ZOrder.overlay,isExcepted=true,},
  { tag = 8,type = RoleType.robject,frame = "cloud21.png",pos = Vec2(34,4), offset=Vec2(0,0),  direction=MovingDirection.toLeft,  allowGravity=false,zorder=ZOrder.cover,onCheckTile=true, isSolid=false,isHarmful=false,step=stepCloud,distance=Vec2(ScreenWidth*2.2,ScreenHeight), allowFlip=false,isExcepted=true,}, 
  --sun
  { tag = 9,type = RoleType.robject,frame = "sun1.png", frameIndexes=FrameTemplate.sun,pos = Vec2(3,4), offset=Vec2(0,0),  allowGravity=false, onCheckTile=false, isSolid=false,sound=Sound.sting, allowFlip=false, gravity=Vec2(-0.05,-0.05), animate=Animate.hoverSun,},
    { tag = 10,type = RoleType.robject,frame = "roadsign.png",pos = Vec2(19,8), offset=Vec2(0,-18),  allowGravity=false,zorder=ZOrder.cover,onCheckTile=false, isSolid=false,  allowFlip=false, direction= MovingDirection.stayStill,step=Vec2(3,3),isExcepted=true,},
 
    { tag = 11,type = RoleType.robject,file = Folder.texture.."dialog_cloud.png",pos = Vec2(-3,5), offset=Vec2(0,0),  allowGravity=false,onCheckTile=false, isSolid=false,  allowFlip=false, direction= MovingDirection.toRight,step=Vec2(2.5,2.5),isExcepted=true,distance=Vec2(ScreenWidth*1.2,ScreenHeight),text=Hints.cloudHints[1]},
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
    
    if  Ref.stage:isTypeDone() and Checkpoint.id == 0  then 
   Resh.playMusic(Music.AdventureTime)
    Role[1].fsm:changeState(State14.Npc.Chasing)
    Role[2].fsm:changeState(State14.Npc.Chasing)
    Role[3].fsm:changeState(State14.Npc.Chasing)
    end  
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

    if (Joystick.ref:getTouchedPad(2)) then  
      Role[1].ref:getWeapon():shootAt(Vec2(0,0))
    end
  end
end
preload = function()
  --根据语言载入对话,cpp动态写入语言，必须在函数内执行
  require("dialog".."14-"..Language)
  PopupInfo = require("popup"..Language)
  --预先载入公共事件或属性，比如Role里面的事件
  preloadStage()

  local iter = 1
  Role[iter].fsm = Fsm:new(Role[iter],State14.Player.Routine, nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State14.RObject.Respawn) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State14.RObject.Respawn) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State14.RObject.Respawn) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State14.RObject.Respawn) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State14.RObject.Respawn) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State14.RObject.RoadSign) iter = iter + 1
  --
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  --触发器
  iter = 1 

end
-- cpp初始化调用，
initiate = function() 
  -- Ads.showBanner(Banner)
  Joystick.ref:hideJoystick()
  State14.initalize()
  if (Checkpoint.id == 0) then
   Ref.stage:loadCurtain({type = CurtainType.black, delay = 4, zorder = ZOrder.robject})
 
  else
   Resh.playMusic(Music.AdventureTime)
    Role[1].ref:spawn(Vec2(4,9),Vec2.ZERO)
    Role[RoleIndex.wangba].fsm:changeState(State14.Npc.Command)
    Role[RoleIndex.wangba].ref:spawn(Vec2(13,7),Vec2(0,-15))
    Role[RoleIndex.pickup].ref:spawn(Vec2(14,9),Vec2(0,0)) 
  end
  Role[1].ref:setFrameSpeed(0.1)
  Role[RoleIndex.pickup].ref:setFrameSpeed(0.05)
  globalInit()
end
--循环调用
update = function()
  updateGlobal()  
end