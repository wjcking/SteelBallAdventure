--测试状态机在里面
require("global")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local NpcStates = require("npcstates")
local Telegram = require("telegram")
local CommonStates = require("commonstates")
local State15= require("state15")
PopupInfo = nil
Resources =
{  
  { music = Music.Retro }, 

}
Background =
{
  {file= Folder.background.."night.png",},
}
-- 地图
Atlas =
{
  {file = "map/map15.tmx" , view =  MapView.horizontal,   }  
}
Triggers =
{
  {type=0,name="trap",}
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
  drop="drop",
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
  { file = "control/fire.png",pos = Vec2(ScreenWidth - 70 * 2.5, 45), name = CommandPad.fire, visible= false, }, 
  { file = "control/drop.png",pos = Vec2(ScreenWidth - 70 * 3.5, 45), name = CommandPad.drop,visible= false, },
}

RoleIndex =
{
  magicBrick=4,
  brick1 = 2,
  brick2 = 3,
  switchPhase1Left = 5,
  switchPhase1Right =6,
  spikeTop =7,
  board=8,

  switchPhase2Left = 9,
  switchPhase2Right =10,
  switchPhase2Middle =11,
  slimePhase2 =12,

  questionLeft = 13,
  questionRight= 14,
  slimePhase3 = 15,

  door = 16,
  spiral = 16,
  spikeTopRight =17,
}

local FrameSwitchRed =  {name="switch_red", rangeStart=1,rangeEnd=2, direction=CollisionDirection.intersected, sound = Sound.switch,limitedTimes=-1,  }
local FrameSwitchBlue =  {name="switch_blue", rangeStart=1,rangeEnd=2, direction=CollisionDirection.intersected, sound = Sound.switch,limitedTimes=-1,  }
local FrameSwitchGreenV =  {name="switch_green_v", rangeStart=1,rangeEnd=2,direction=CollisionDirection.intersected,sound = Sound.switch,limitedTimes=1, }
local FrameSwitchRedV_Unlimited =  {name="switch_red_v", rangeStart=1,rangeEnd=2,direction=CollisionDirection.intersected,sound = Sound.switch,limitedTimes=-1, }
local FrameSwitchGreenV_Unlimited =  {name="switch_green_v", rangeStart=1,rangeEnd=2,direction=CollisionDirection.intersected,sound = Sound.switch,limitedTimes=-1, }

Role = 
{
  { tag = 1,type = RoleType.player,frame = "player1.png",pos = Vec2(7,21), frameIndexes=FrameTemplate.player, zorder=ZOrder.cover,allowThrough=false},
  { tag = 2,type = RoleType.robject,tile = Tile.brickRed,pos = Vec2(1,17),  },
  { tag = 3,type = RoleType.robject,tile = Tile.brickRed,pos = Vec2(2,17),  },
  { tag = 4,type = RoleType.robject,tile = Tile.brickRed,pos = Vec2(14,18), allowPush=CollisionDirection.intersected, allowGravity=true,  },
  { tag = 5,type = RoleType.robject,frame = "switch_red1.png",pos = Vec2(1,21),  allowGravity=true, isSolid=false,switch=FrameSwitchRed,allowFlip=false,zorder=ZOrder.cover,},
  { tag = 6,type = RoleType.robject,frame = "switch_green_v1.png",pos = Vec2(16,16), allowGravity=false, isSolid=false, switch=FrameSwitchGreenV,allowFlip=false,zorder=ZOrder.cover,},
  { tag = 7,type = RoleType.robject,file = Folder.texture.."spiketop4.png",pos = Vec2(4,16), offset=Vec2(70,11), allowGravity=false, isSolid=false,allowFlip=false,sound=Sound.sting,onCheckTile=false,isHarmful=true,},
  { tag = 8,type = RoleType.robject,frame = "dialog_info.png",pos = Vec2(9,18), allowGravity=false, isSolid=false,  allowFlip=false, text=RObjectText.board1,zorder=ZOrder.cover - 1, onCheckTile=false,},
  --第二阶段
  { tag = 9,type = RoleType.robject,frame = "switch_green_v1.png",pos = Vec2(1,27), allowGravity=false, isSolid=false, switch=FrameSwitchGreenV_Unlimited, offset=Vec2(-2,0),switchResult=FrameSwitch() },
  { tag = 10,type = RoleType.robject,frame = "switch_red_v1.png",pos = Vec2(16,27), allowGravity=false, isSolid=false, switch=FrameSwitchRedV_Unlimited, offset=Vec2(2,0) ,switchResult=FrameSwitch() },
  { tag = 11,type = RoleType.robject,frame = "switch_blue1.png",pos = Vec2(8,28), allowGravity=false, isSolid=false, switch=FrameSwitchBlue,switchResult=FrameSwitch()},
  { tag = 12,type = RoleType.npc,frame = "bigslime_red1.png",framePrefix="bigslime_red", pos = Vec2(14,31),  isSolid=false, frameIndexes=FrameTemplate.slime, offset=Vec2(0,0) ,face=MovingDirection.toLeft},
  --第三阶段
  { tag = 13,type = RoleType.robject,frame = "question_red.png",pos = Vec2(4,36), allowGravity=false, isSolid=false, offset=Vec2(0,0),allowTouch =true,allowDragX=true },
  { tag = 14,type = RoleType.robject,frame = "question_blue.png",pos = Vec2(13,36), allowGravity=false, isSolid=false,  offset=Vec2(0,0),hp=100,hpMax=100},
  { tag = 15,type = RoleType.npc,frame = "bigslime_gray1.png",pos = Vec2(14,39), framePrefix="bigslime_gray", isSolid=false, frameIndexes=FrameTemplate.slime, offset=Vec2(0,0) ,face=MovingDirection.toLeft,allowPatroll=false,step=Vec2.ZERO},
  {tag=16,type = RoleType.robject,frame = "door1.png", isSolid=false, offset=Vec2(20,0),  pos = Vec2(8,8), isSolid=false,allowGravity=true,insetX=24,  allowFlip=false,},
  --后加
    { tag = 17,type = RoleType.robject,file = Folder.texture.."spiketop4.png",pos = Vec2(10,16), offset=Vec2(70,11), allowGravity=false, isSolid=false,allowFlip=false,sound=Sound.sting,onCheckTile=false,isHarmful=false, visible=false,},
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
    --掉包
     if (Joystick.ref:getTouchedPad(3)) then 
      Role[1].ref:dropObject()
    end
  end
end
preload = function()
  --根据语言载入对话,cpp动态写入语言，必须在函数内执行
  require("dialog".."15-"..Language)
  PopupInfo = require("popup"..Language)
  --预先载入公共事件或属性，比如Role里面的事件
  preloadStage()

  local iter = 1
  Role[iter].fsm = Fsm:new(Role[iter],State15.Player.Routine, nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1 
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
--phase2
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  --phase3
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1

  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  --后加 phase1 spkie
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  --触发器
  iter = 1 
  Triggers[iter].fsm = Fsm:new(Triggers[iter],nil, State15.Triggers.Trap) iter = iter + 1
  ----释放
 
end
-- cpp初始化调用，
initiate = function() 
    Ref.stage:loadCurtain({type = CurtainType.bilateral, image={"background/curtainleft.png","background/curtainright.png"},  delay = 1, zorder = ZOrder.dropCurtain})
  Resh.playMusic(Music.Retro)
  Role[RoleIndex.board].fsm:changeState(State15.RObject.Board)
  Role[RoleIndex.switchPhase2Left].ref.isFlippedX = true
   
  if (Checkpoint.id == 0) then
    getStage():setPtc(Vec2(0,23),CameraSlideOffset)
    Role[1].ref:spawn(Vec2(7,21),Vec2.ZERO)
    Role[RoleIndex.magicBrick].fsm:changeState(State15.RObject.MagicBrick)
    Role[RoleIndex.switchPhase1Left].fsm:changeState(State15.RObject.Switch)
    Role[RoleIndex.switchPhase1Right].fsm:changeState(State15.RObject.Switch)
    Role[RoleIndex.spikeTop].fsm:changeState(State15.RObject.Spikes)
 
  elseif (Checkpoint.id == 1) then
    getStage():setPtc(Vec2(0,33),CameraSlideOffset)
    Role[1].ref:spawn(Vec2(7,31),Vec2.ZERO)
    Role[1].ref:setZOrder(ZOrder.role)
    Role[RoleIndex.board].ref:registerText(RObjectText.board2)
    Role[RoleIndex.board].ref:spawn(Vec2(8,26),Vec2(10,15))
    Role[RoleIndex.board].ref:setAllowTouch(true)
    Role[RoleIndex.board].ref:setAllowDragX(true)
    Role[RoleIndex.switchPhase2Left].fsm:changeState(State15.RObject.Switch2)
    Role[RoleIndex.switchPhase2Right].fsm:changeState(State15.RObject.Switch2)
    Role[RoleIndex.switchPhase2Middle].fsm:changeState(State15.RObject.Switch2)
    Role[RoleIndex.slimePhase2].fsm:changeState(State15.Npc.Slime)
  elseif (Checkpoint.id == 2) then
    getStage():setPtc(Vec2(0,43),CameraSlideOffset)
    Role[1].ref:spawn(Vec2(8,39),Vec2.ZERO)
    Role[RoleIndex.board].ref:registerText(RObjectText.board3)
    Role[RoleIndex.board].ref:spawn(Vec2(8,36),Vec2(10,15))
    Role[RoleIndex.board].ref:setAllowDragX(false)

    Role[RoleIndex.questionLeft].fsm:changeState(State15.RObject.Question)
    Role[RoleIndex.questionRight].fsm:changeState(State15.RObject.Question) 
    Role[RoleIndex.slimePhase3].fsm:changeState(State15.Npc.Slime)
  elseif (Checkpoint.id == 3) then
    Role[1].ref:spawn(Vec2(7,39),Vec2.ZERO)   
    getStage():setPtc(Vec2(0,43),CameraSlideOffset)
    State15.loadPhase3()
  end
  
 
   globalInit()
end
--循环调用
update = function()

  if (checkRetry()) then
    updateGlobal()
  end

end