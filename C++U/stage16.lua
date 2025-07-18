--测试状态机在里面
require("global")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local NpcStates = require("npcstates")
local Telegram = require("telegram")
local CommonStates = require("commonstates")
local State16= require("state16")
PopupInfo = nil
Resources =
{  
  { music = Music.Boss }, 
 
}
Background =
{
  {file= Folder.background.."night.png",},
}
-- 地图
Atlas =
{
  {file = "map/map16.tmx" , view =  MapView.horizontal, image=Folder.background.."stage16.png"  }  
}
Triggers =
{

}

--对话范围 cpp下标 0 开始
Segment =
{
  { rangeStart = 0,  rangeEnd= 7, },
  { rangeStart = 8,  rangeEnd= 12, },
  { rangeStart = 13,  rangeEnd= 26,},
}

--如果lua中上一次的collapse存在，务必重置后才能使用否则会出现range越界,定义变量不赋值则可以清空
Collapse =
{

}
CommandPad = 
{
  jump = "jump",
  barDick="barDick",
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
  { file = "control/fire.png",pos = Vec2(ScreenWidth - 70*2.5, 45), name = "fire", text="",visible=true,},
  { file = "control/bar.png",pos = Vec2(ScreenWidth/2, ScreenHeight-20), name = "barDick", text="",visible=true},
}

RoleIndex =
{
  agnail=3,
  dick=2,
  zzi=6,
  
  missileRed=7,
  missileBlue =8,
}
local function updateTileSection()
  local npc = Role[RoleIndex.dick]
  local boundNpc = npc.ref:getBoundSelf().tilePosition
  if (boundNpc.x  == 2) then
    npc.sectionLeft = npc.sectionLeft  +1
  elseif (boundNpc.x == 15) then
    npc.sectionRight = npc.sectionRight  +1
  end
end
local function shoot()
  local steelball = Role[1].ref:getBoundSelf().tilePosition
  local dick =  Role[RoleIndex.dick].ref:getBoundSelf().tilePosition
  if  math.abs(steelball.x  -dick.x) >= 8 and  math.random(12) == 12  and
  (
    ( Role[RoleIndex.dick].ref.face == MovingDirection.toLeft and Role[1].ref.face == MovingDirection.toRight) or 
  ( Role[RoleIndex.dick].ref.face == MovingDirection.toRight and Role[1].ref.face == MovingDirection.toLeft) 
  )then
     Role[RoleIndex.dick].ref:getWeapon():shootAt(Vec2.ZERO)
  end
end
local function isDickReady(times)
  if (times == nil) then
    times = 2
  end
  local npc = Role[RoleIndex.dick]
--  local playerFacing = Role[1].ref.face
--  local dickFacing = Role[RoleIndex.dick].ref.face
  local flag = false
  --如果dick在钢蛋右面
--  if npc.ref:getPositionX() >=  Role[1].ref:getPositionX() then
    if  npc.sectionRight ~=0 and npc.sectionRight % 62 == times  then
      npc.ref.face = MovingDirection.toLeft
      npc.ref.isFlippedX = true
      flag = true
      
    end
--  else
--    if (npc.sectionLeft % 62 == times) then
--      flag = true
--    end
 -- end
 --重置
  if (flag and npc.sectionRight >= 62 * (times + 1)) then
    npc.sectionRight  = 0
    npc.sectionLeft  = 0
  end
  return flag
end
local hover = {type = Anistyle.rebound,limitedTimes=-1, offset=Vec2(0,12), duration=2, durationBack=2, delay=0,delayBack=0} 
local  gunPoo= {  count=-1,  sound=Sound.poogun,rate=1, frame="poo.png",project={type=ProjectType.object, allowGravity=false,step=Vec2(2.5,2.5), }}

local  gunObject= {  count=-1,  sound=Sound.toygun,rate=0.0066, frame="bullet1.png",project={type=ProjectType.object, allowGravity=false, step=Vec2(6,6)},}
Role = 
{
  { tag = 1,type = RoleType.player,frame = "player1.png",pos = Vec2(4,19), frameIndexes=FrameTemplate.player, weapon=gunObject,zorder=ZOrder.cover, hp=6,hpMax=6,},
  { tag = 2,type = RoleType.npc,frame ="wb1.png", frameIndexes=FrameTemplate.wb,pos = Vec2(14,19), offset=Vec2(0,0),   face=MovingDirection.toLeft,  hp=100,hpMax=100,weapon=gunPoo, sectionLeft=0,sectionRight=0, updateTileSection=updateTileSection, isReady=isDickReady,allowRecover = true,shoot=shoot,},
  { tag = 3,type = RoleType.npc,frame ="agnail_gray1.png",  pos = Vec2(6,8), offset=Vec2(-20,0),   face=MovingDirection.toLeft, isHarmful=true, allowGravity=false, onCheckTile=false,isExcepted=false, animate=hover,},  
  { tag = 4,type = RoleType.npc,frame ="bug_blue2.png",  pos = Vec2(9,8), offset=Vec2(0,0),   face=MovingDirection.toLeft, isHarmful=true,  allowGravity=false, onCheckTile=false,isExcepted=false,animate=hover,},  
  { tag = 5,type = RoleType.npc,frame ="bigslime_green3.png",  pos = Vec2(12,8), offset=Vec2(0,0),   face=MovingDirection.toLeft,  isHarmful=true,  allowGravity=false, onCheckTile=false,isExcepted=false,animate=hover,},  
  { tag = 6,type = RoleType.npc,frame ="zzi2.png",  pos = Vec2(15,8), offset=Vec2(0,0),   face=MovingDirection.toLeft,  isHarmful=false,  allowGravity=false, isExcepted=true,animate=hover, step=Vec2(2,2), onCheckTile=true, },
  { tag = 7,type = RoleType.robject,frame ="missile_red.png",  pos = Vec2(22,15), offset=Vec2(0,-2),   face=MovingDirection.toLeft,  isHarmful=false,  allowGravity=false, zorder=ZOrder.cover,direction=MovingDirection.stayStill, isSolid=true,allowCollision=CollisionDirection.atTop, step=Vec2(1,1),insetY=4, onCheckTile=false,},  
  { tag = 8,type = RoleType.robject,frame ="missile_blue.png",  pos = Vec2(18,18), offset=Vec2(0,25),   face=MovingDirection.toLeft,  isHarmful=false,  allowGravity=false, zorder=ZOrder.cover,direction=MovingDirection.stayStill, isSolid=true,allowCollision=CollisionDirection.atTop,step=Vec2(1,1),insetY=4,onCheckTile=false,},  
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

    if (Joystick.ref:getTouchedPad(2)) then  
      Role[1].ref:getWeapon():shootAt(Vec2(0,0))
    end
  end
end
preload = function()
  --根据语言载入对话,cpp动态写入语言，必须在函数内执行
  require("dialog".."16-"..Language)
  PopupInfo = require("popup"..Language)
  --预先载入公共事件或属性，比如Role里面的事件
  preloadStage()

  local iter = 1
  Role[iter].fsm = Fsm:new(Role[iter],State16.Player.Routine, nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1


  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  --zzi
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1

  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  --触发器
  iter = 1 

end
-- cpp初始化调用，
initiate = function() 
  --Ads.showBanner(Banner)
  State16.initalize()
    Ref.stage:loadCurtain({type = CurtainType.bilateral, image={"background/leftrage.png","background/rightdick.png"},  delay = 1, zorder = ZOrder.dropCurtain})
  Joystick.ref:setString(CommandPad.barDick,RObjectText.dickHP..Role[RoleIndex.dick].ref.hp)

  if (Checkpoint.id == 0 ) then
    Segment[1].fsm:changeState(State16.Dialogue.Dialog)
  else
  Role[1].ref:spawn(Vec2(4,19),Vec2.ZERO)
  Role[RoleIndex.dick].fsm:changeState(State16.Dick.Patrol) 
  Role[3].fsm:changeState(State16.Npc.Slime)
  Role[4].fsm:changeState(State16.Npc.Slime)
  Role[5].fsm:changeState(State16.Npc.Slime)
  Role[RoleIndex.zzi].fsm:changeState(State16.Npc.Zzi)
  Role[RoleIndex.missileRed].fsm:changeState(State16.RObject.Missile)
  Role[RoleIndex.missileBlue].fsm:changeState(State16.RObject.Missile)
end
globalInit()
end
--循环调用
update = function()
  if (checkRetry()) then
 --Ads.updateBanner(1,0,0)
    updateGlobal()
    clearDisposed()
  end

end