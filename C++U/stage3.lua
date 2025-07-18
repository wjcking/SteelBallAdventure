--测试状态机在里面
require("global")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local NpcStates = require("npcstates")
local Telegram = require("telegram")

local State3 = require("state3")
PopupInfo = nil
Resources =
{  
  {music = Music.SaltyDitty},
}
Background =
{
  {file= Folder.background.."orange.png",},
}
-- 地图
Atlas =
{
  {file = "map/map3.tmx" , view =  MapView.horizontal,   image=Folder.background.."wood2.png" }
}
Triggers =
{
  {type=0, name="spike",  isActive=true}, 
  -- {type=0, name="drop",  isActive=true}, 
}
--对话范围 cpp下标 0 开始
Segment =
{
  { rangeStart = 0,  rangeEnd= 0, },
}


--如果lua中上一次的collapse存在，务必重置后才能使用否则会出现range越界,定义变量不赋值则可以清空
Collapse =
{

}
CommandPad = 
{
  jump = "jump",
  drop = "drop",
}
Joystick =
{
  type = 2,
  controlTag = 1,
}
Joypad =
{
  { file = "control/jump.png",pos = Vec2(ScreenWidth - 70, 45), name = CommandPad.jump },
  { file = "control/drop.png",pos = Vec2(ScreenWidth - 70 * 2.5, 45), name = CommandPad.drop,isVisible= false, },
}

RoleIndex =
{
  bigSlime = 2,
  fruitShaking = 3,
  fruitRed = 4,
  fruitGreen = 5,
  fakeCheck = 7,
  spikeBackward1=12,
  spikeBackward2=13,
  block1 = 11,
  block2 =9,
  block3=10,
  block4=8,
  dialogBoard=6,
}
local knocks =
{
  { type=KnockAction.popup,limitedTimes=1, direction=CollisionDirection.atBottom,tile=Tile.grayBrick, sound= Sound.coin,toast=Animate.toastCoin},
}
local hidden=
{
  { type=KnockAction.hidden, direction=CollisionDirection.atBottom,tile=Tile.grayBrick, sound= Sound.coin,toast=Animate.toastCoin},
}
Role = 
{
  { tag = 1,type = RoleType.player,frame = "player1.png",pos = Vec2(2,12) },
  { tag = 2,type = RoleType.npc,frame = "bigslime_red1.png", framePrefix="bigslime_red",pos = Vec2(26,14), face=MovingDirection.toLeft, weapon = {count=-1,  sound=Folder.sound.."toygun.wav",rate=1.5, frame="bullet1.png",project={type=ProjectType.object,step=Vec2(3,3),allowGravity=false, }}, frameIndexes=FrameTemplate.slime},
  { tag = 3, type = RoleType.robject,frame = "fruit_green.png",pos = Vec2(11,14), onCheckTile=false,isSolid=false,allowGravity=false, animate=Animate.jumpShake,   gravity=Vec2(0,-0.135),},
  {tag = 4, type = RoleType.robject,frame = "fruit_red.png",pos = Vec2(13,12), isSolid=false, allowTaken = true,allowGravity=true,},
  {tag = 5, type = RoleType.robject,frame = "fruit_green.png",pos = Vec2(15,14), isSolid=false},
  {tag = 6, type = RoleType.robject,frame = "dialog_info.png",pos = Vec2(6,13),   text = RObjectText.notice, replacedText=RObjectText.playerDead, isSolid = false,  zorder=1, touchedText = RObjectText.touched, allowTouch = true, },
  {tag =7, type = RoleType.robject,tile = Tile.blockRed,pos = Vec2(26,12), knocks=hidden, onCheckTile=false, allowGravity=false,},

  { tag =8,type = RoleType.robject,tile = Tile.blockRed,pos = Vec2(41,12),onCheckTile=false, knocks = knocks,},
  { tag =9,type = RoleType.robject,tile = Tile.blockRed,pos = Vec2(43,12),onCheckTile=false, knocks = knocks,},
  {tag =10,type = RoleType.robject,tile = Tile.blockRed,pos = Vec2(41,15),onCheckTile=false, knocks = knocks,},
  {tag =11,type = RoleType.robject,tile = Tile.blockRed,pos = Vec2(43,15),onCheckTile=false, knocks = knocks,},
  {tag =12,type = RoleType.robject,tile = Tile.spikeLeft,pos = Vec2(44,13), step=Vec2(4,4),insetX=24,sound=Sound.sting, isHarmful=true, onCheckTile=false,isSolid=false,allowGravity=false,allowFlip=false,},
  {tag =13,type = RoleType.robject,tile = Tile.spikeLeft,pos = Vec2(44,14),step=Vec2(4,4),insetX=24,sound=Sound.sting, isHarmful=true, onCheckTile=false,isSolid=false,allowGravity=false,allowFlip=false,},
  {tag =13,type = RoleType.robject,frame = "spiral.png", isSolid=false, offset=Vec2(0,10), animate=Animate.rotateSelf, pos = Vec2(52,17), onCheckTile=false,isSolid=false,allowGravity=false,insetX=24,},

}


--回调事件，funcName函数名
callback = function(className,funcName,params) 
  --全局回调函数
  globalCallFunc(className,funcName,params)
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
      Role[1].ref:dropObject()
    end
  end
end

preload = function()
  --根据语言载入对话,cpp动态写入语言，必须在函数内执行
  require("dialog".."3-"..Language)
  PopupInfo = require("popup"..Language)
  --预先载入公共事件或属性，比如Role里面的事件
  preloadStage()

  local iter = 1
  Role[iter].fsm = Fsm:new(Role[iter],State3.Player.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],NpcStates.Routine,State3.Npc.BigSlime) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State3.RObject.FruitRed) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State3.RObject.Fall) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,RObjectStates.DialogBoard) iter = iter + 1
  --hidden
  Role[iter].fsm = Fsm:new(Role[iter],nil,RObjectStates.Knocked) iter = iter + 1 
  --第二阶段 
  --4个砖块
  Role[iter].fsm = Fsm:new(Role[iter],nil,State3.RObject.Knocked) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State3.RObject.Knocked) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State3.RObject.Knocked) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State3.RObject.Knocked) iter = iter + 1

  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  --终点
  Role[iter].fsm = Fsm:new(Role[iter],nil,RObjectStates.Destiny) iter = iter + 1
  iter = 1
  Triggers[iter].fsm = Fsm:new(Triggers[iter],nil,State3.Triggers.Spike)  iter = iter + 1
  -- Triggers[iter].fsm = Fsm:new(Triggers[iter],nil,State3.Triggers.Drop)  iter = iter + 1
end
-- cpp初始化调用，
initiate = function()
  Ref.stage:loadCurtain({type = CurtainType.bilateral, image={"background/curtainleft.png","background/curtainright.png"},  delay = 1, zorder = ZOrder.dropCurtain})
  Resh.playMusic( Music.SaltyDitty)

  Role[RoleIndex.fruitRed].ref:setGravityOff()

  State3.RObject.Knocked.init()
  --checkpoint
  if Checkpoint.id > 0 then
    Role[1].ref:spawn(Checkpoint.position,Vec2.ZERO)
     Role[RoleIndex.dialogBoard].ref:registerText(RObjectText.notice2)
    Role[RoleIndex.dialogBoard].ref:spawn(Vec2(34,13),Vec2.ZERO)
  end
  createStar(2,Vec2(49,10),ZOrder.robject)
  globalInit()
end
--循环调用
update = function()
  if (checkRetry()) then
    updateGlobal()
  end
end