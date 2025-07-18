--测试状态机在里面
require("global")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local NpcStates = require("npcstates")
local CommonStates = require("commonstates")
local Telegram = require("telegram")
local State9 = require("state9")
PopupInfo = nil
Resources =
{  
  {sound=Sound.block1},
  {sound=Sound.block2},
  {sound=Sound.howling},
  {music=Music.AJourneyAwaits},
}
Background =
{
  {file= Folder.background.."stage9.png",},
}

-- 地图
Atlas =
{
  {file = "map/map9.tmx", view = MapView.panorama} 
}
Triggers =
{

}
--对话范围 cpp下标 0 开始
Segment =
{
  { rangeStart = 0,  rangeEnd= 4, },

}
--如果lua中上一次的collapse存在，务必重置后才能使用否则会出现range越界,定义变量不赋值则可以清空
Collapse =
{

}

Joystick = { type = 1, controlTag = 1, visible=false,}
CommandPad = 
{
  barSteelball="barSteelball",
  barDick ="barDick",
  pad1 = "1",
  pad2 = "2",
  pad3 = "3", 
  wall = "barWall",
  fire="fire",
}

Joypad =
{
  { file = "control/bar.png",pos = Vec2(ScreenWidth/2, ScreenHeight-20), name = "barWall", text="",visible=true},

  { file = "control/fire.png",pos = Vec2(ScreenWidth - 85, 50),     name = "fire", text="",visible=false,},
  { file = "control/3.png",pos = Vec2(ScreenWidth - 85 * 2, 50), name = "3", text="",visible=false,},
  { file = "control/2.png",pos = Vec2(ScreenWidth - 85 * 3, 50), name = "2", text="",visible=false,},
  { file = "control/1.png",pos = Vec2(ScreenWidth - 85 * 4, 50), name = "1", text="",visible=false,} ,
}

RoleIndex =
{
  wangba=2,
  wall=4,
  flag=5,
}
local  gunDeadshot= {  count=-1,  sound=Folder.sound.."fire.wav",rate=0.8, frame="poo.png",project={type=ProjectType.deadshot, allowGravity=false,step=Vec2(11,11),target=1,}}

local  gunObject= {  count=-1,  sound=Sound.toygun,rate=0.66, frame="bullet1.png",project={type=ProjectType.object, delay=3,range=Vec2(300,300),step=Vec2(6,6)}}
Role = 
{
  { tag = 1,type = RoleType.player,frame = "player1.png",pos = Vec2(1,8), insetTile = Vec2(13.0,3.0),offset=Vec2(-10,5), frameIndexes=FrameTemplate.player, text= Hints.ballCommand, lineIndex=2, zorder=10, weapon=gunObject,isFired=false,hp=3,hpMax=3},
  { tag = 2,type = RoleType.npc,frame = "wb1.png", framePrefix="wb",pos = Vec2(15,4), offset=Vec2(-4,0),face=MovingDirection.toLeft,frameIndexes=FrameTemplate.wb,text= Hints.wangbaCommand,hp=1000,hpMax=1000,weapon=gunDeadshot},

  { tag = 3,type = RoleType.robject,file=Folder.texture.."tower.png",pos = Vec2(15,6), onCheckTile=false,isSolid=false, zorder=5,isExcepted=true,},

  { tag = 4,type = RoleType.robject, frame="wall1.png", hp=250,hpMax=250,pos = Vec2(15,8),zorder=6, offset=Vec2(20,34), onCheckTile=false,isSolid=false, },
  { tag = 5,type = RoleType.robject, file=Folder.texture.."flag_dick.png",pos = Vec2(16,6),zorder=6, offset=Vec2(-8,25), onCheckTile=false,isSolid=false, },
  { tag = 6,type = RoleType.npc, frame="bird_gray1.png", step=Vec2(0.3,0.3),direction=MovingDirection.toLeft, face=MovingDirection.toLeft,framePrefix="bird_gray",pos = Vec2(18,3), offset=Vec2(0,0), isSolid=false, isHarmful=false,frameIndexes=FrameTemplate.bird,weapon=gunDeadshot},
  { tag = 7,type = RoleType.npc, frame="bird_gray1.png",step=Vec2(0.3,0.3), direction=MovingDirection.toRight,face=MovingDirection.toLeft,framePrefix="bird_gray",pos = Vec2(0,4),  offset=Vec2(0,0), isSolid=false,  isHarmful=false,frameIndexes=FrameTemplate.bird,weapon=gunDeadshot},
}
BallCounter = 0
SlimeCounter = 0
BallStep= Vec2(0.15,0.15)
SlimeStep= Vec2(0.15,0.15)
BallMax = 10
SlimeMax = 10
DispatchInterval = 1.01

local allowDispatch = true
local function dispatchBalls()
  local touches = Joystick.ref:getTouches()

  if (#touches ~= 1 or not allowDispatch or BallCounter >= BallMax or touches[1] == CommandPad.fire) then
    return
  end  
  Role[1].fsm:changeState(State9.Npc.Command) 
  local frame="kid1.png"
  local framePrefix="kid"
  local pos = Vec2(2,7)
  local offset =Vec2(0,0)
  local lineIndex = 1
  if (touches[1] == CommandPad.pad1) then
    frame="kid1.png"
    framePrefix="kid"
    lineIndex = 1
    pos = Vec2(2,7) 
  elseif (touches[1] == CommandPad.pad2) then
    frame="cowboy1.png"
    framePrefix="cowboy"
    lineIndex = 2
    pos = Vec2(2,8) 
  elseif (touches[1] == CommandPad.pad3) then
    frame="man1.png"
    framePrefix="man"
    lineIndex = 3
    pos = Vec2(2,9) 
    offset=Vec2(0,5) 
  end
  local  gunObject= {  count=-1,  sound=Sound.toygun,rate=1, frame="bullet1.png",project={type=ProjectType.object, delay=15,range=Vec2(300,300),step=Vec2(3,3)}}
  local ball = { type=RoleType.player, pos=pos,frame=frame, framePrefix=framePrefix,frameIndexes=FrameTemplate.player,offset=offset, step=BallStep, onCheckTile=false, isSolid=false, weapon=gunObject, zorder=6, lineIndex=lineIndex,}
  ball.fsm = Fsm:new(ball,State9.Npc.Routine,State9.Player.Balls)
  createRole(ball)
  allowDispatch = false
  BallCounter = BallCounter + 1
  Resh.playSound(Sound.button)
end

local function dispatchSlime()
end

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
  if (isFreezed()) then
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
    dispatchBalls()
    if (Joystick.ref:getTouches()[1] == CommandPad.fire) then  
      Role[1].ref:getWeapon():shootAt(Vec2.ZERO)
      Role[1].isFired = true
    end
  end

end

preload = function()
  --根据语言载入对话,cpp动态写入语言，必须在函数内执行
  require("dialog".."9-"..Language)
  PopupInfo = require("popup"..Language)
  --预先载入公共事件或属性，比如Role里面的事件
  preloadStage()
  local iter = 1
  Role[iter].fsm = Fsm:new(Role[iter],State9.Player.Routine,nil) iter = iter + 1

  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1 
  Role[iter].fsm = Fsm:new(Role[iter],nil,State9.RObject.Wall) iter = iter + 1

  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1 
  Role[iter].fsm = Fsm:new(Role[iter],nil,CommonStates.Respawn) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,CommonStates.Respawn) iter = iter + 1

  iter = 1

end
-- cpp初始化调用，
initiate = function()
--    Ref.stage:loadCurtain({type = CurtainType.bilateral, image={"background/leftrage.png","background/rightdick.png"},  delay = 1, zorder = ZOrder.dropCurtain})
  Joystick.ref:hideRocker()
  Joystick.ref:setString(CommandPad.wall,tostring(Hints.barWall..Role[RoleIndex.wall].hp))
  if (Checkpoint.id == 0) then
    Segment[1].fsm:changeState(State9.Dialogue.Start)
  else

    Role[1].ref:spawn(Vec2(1,8),Vec2(-10,5))
    Joystick.ref:setVisible(CommandPad.pad1,true)
    Joystick.ref:setVisible(CommandPad.pad2,true)
    Joystick.ref:setVisible(CommandPad.pad3,true) 
    Joystick.ref:setVisible(CommandPad.fire,true) 
    Role[1].fsm:changeState(State9.Npc.Command) 
    Role[RoleIndex.wangba].fsm:changeGlobal(State9.Npc.Wangba)
    Role[RoleIndex.wangba].fsm:changeState(State9.Npc.Command)
  end
  globalInit()
end
--循环调用

update = function() 
  --计算是否能添加ball,时间间隔
  if ( Delay.isTimeUp(DelayIndex.dispatch,DispatchInterval)) then
    if (not allowDispatch) then  
      allowDispatch = true
    end
    Delay.reset(DelayIndex.dispatch)
  end
  --slime,时间间隔，比在execute里面准确，否则会一股脑添加所有slime
  if ( Delay.isTimeUp(DelayIndex.dispatchSlime,2)) then

    if (not State9.allowDispatch) then  
      State9.allowDispatch = true
    end

    Delay.reset(DelayIndex.dispatchSlime)
  end

  if(checkRetry()) then
    updateGlobal()
  end
end