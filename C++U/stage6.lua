--测试状态机在里面
require("global")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")

local CommonStates = require("commonstates")
local Telegram = require("telegram")
local State6 = require("state6")
PopupInfo = nil
Resources =
{  
  {music = Music.BitShift},
  {sound= Sound.toygun },
}
Background =
{
--  {file= Folder.background.."sky.png",},
}

-- 地图
Atlas =
{
  {file = "map/map6.tmx", view = MapView.panorama} 
}
Triggers =
{
  {type=0, name="spike1",  isActive=true}, 
  {type=0, name="check",  isActive=true}, 
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

Joystick = { type = 1, controlTag = 1, }
CommandPad = 
{
  fire = "fire"
}

Joypad =
{
  { file = "control/fire.png",pos = Vec2(ScreenWidth - 85, 40), name = "fire", text="",visible=false },
}

RoleIndex =
{  
  agnail9 = 9,
  agnail13 = 13,
  questionTop = 14,
  questionLeft = 29,
  questionMiddle = 30,
  questionRight =31, 
  saw = 32,
  agnailMiddle1=18,
  agnailMiddle2=25,
  flatBoard=45,
}

 
  local ReboundLeft= {type = Anistyle.rebound,limitedTimes=-1, offset=Vec2(-100,0), duration=1.8, durationBack=1.8, delay=0,delayBack=0, allowBack=true, sound=Folder.sound..""}
   local ReboundMiddle= {type = Anistyle.rebound,limitedTimes=-1, offset=Vec2(0,100), duration=1.8, durationBack=1.8, delay=0,delayBack=0, allowBack=true, sound=Folder.sound..""}
  local ReboundRight= {type = Anistyle.rebound,limitedTimes=-1, offset=Vec2(100,0), duration=1.8, durationBack=1.8, delay=0,delayBack=0, allowBack=true, sound=Folder.sound..""}
local HoverCoin = { type = Anistyle.rebound,delay=0,offset= Vec2(0,-2), limitedTimes=-1}
Role = 
{
  { tag = 1,type = RoleType.player,frame = "player1.png",pos = Vec2(8,30),face = MovingDirection.toTop,insetTile = Vec2(13.0,3.0) },
  { tag = 2,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(3,28), offset=Vec2(-18,15), step=Vec2(0.2,0.2), onCheckTile=false,animate=ReboundLeft,  sound=Sound.sting,},
  { tag = 3,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(5,28), offset=Vec2(-18,15), step=Vec2(0.2,0.2), onCheckTile=false,animate=ReboundLeft, sound=Sound.sting,},
  { tag = 4,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(7,28), offset=Vec2(-18,15), step=Vec2(0.2,0.2), onCheckTile=false,animate=ReboundLeft,  sound=Sound.sting,},
  { tag = 5,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(9,28), offset=Vec2(-18,15), step=Vec2(0.2,0.2), onCheckTile=false,animate=ReboundMiddle, sound=Sound.sting,},
  { tag = 6,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(11,28),offset=Vec2(-18,15), step=Vec2(0.2,0.2), onCheckTile=false,animate=ReboundRight, sound=Sound.sting,},
  { tag = 7,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(13,28),offset=Vec2(-18,15), step=Vec2(0.2,0.2), onCheckTile=false,animate=ReboundRight, sound=Sound.sting,},
  { tag = 8,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(15,28),offset=Vec2(-18,15), step=Vec2(0.2,0.2), onCheckTile=false,animate=ReboundRight, sound=Sound.sting,}, 
  --第二部分
  { tag = 9, type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(3,25), offset=Vec2(-18,0),  step=Vec2(0.2,0.2),onCheckTile=false, sound=Sound.sting,},
  { tag = 10,type = RoleType.npc,frame = "agnail_red1.png",pos = Vec2(6,25), offset=Vec2(-18,0),  step=Vec2(0.2,0.2),onCheckTile=false, sound=Sound.sting,},
  { tag = 11,type = RoleType.npc,frame = "agnail_red1.png",pos = Vec2(9,25), offset=Vec2(-18,0),  step=Vec2(0.2,0.2),onCheckTile=false, sound=Sound.sting,},
  { tag = 12,type = RoleType.npc,frame = "agnail_red1.png",pos = Vec2(12,25),offset=Vec2(-18,0),  step=Vec2(0.2,0.2),onCheckTile=false, sound=Sound.sting,},
  { tag = 13,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(15,25),offset=Vec2(-18,0),  step=Vec2(0.2,0.2),onCheckTile=false, sound=Sound.sting,},
  --问号
  { tag = 14,type = RoleType.robject,frame = "question_red.png",pos = Vec2(8,24),offset=Vec2(20,15), isSolid=false, onCheckTile=false,  },
  --第二阶段 array1
  { tag = 15,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(3,15), offset=Vec2(-18,15), step=Vec2(0.2,0.2), isSolid=false, sound=Sound.sting,}
  , 
  { tag = 16,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(5,15), offset=Vec2(-18,15), step=Vec2(0.2,0.2), isSolid=false, sound=Sound.sting,},
  { tag = 17,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(7,15), offset=Vec2(-18,15), step=Vec2(0.2,0.2), isSolid=false, sound=Sound.sting,},
  { tag = 18,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(9,15), offset=Vec2(-18,15), step=Vec2(0.2,0.2), isSolid=false, sound=Sound.sting, },
  { tag = 19,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(11,15),offset=Vec2(-18,15), step=Vec2(0.2,0.2), isSolid=false, sound=Sound.sting,},
  { tag = 20,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(13,15),offset=Vec2(-18,15), step=Vec2(0.2,0.2), isSolid=false, sound=Sound.sting,},
  { tag = 21,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(15,15),offset=Vec2(-18,15), step=Vec2(0.2,0.2), isSolid=false, sound=Sound.sting,}, 
  --array2
  { tag = 22,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(3,14), offset=Vec2(-18,15), step=Vec2(0.2,0.2), isSolid=false, sound=Sound.sting,},
  { tag = 23,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(5,14), offset=Vec2(-18,15), step=Vec2(0.2,0.2), isSolid=false, sound=Sound.sting,},
  { tag = 24,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(7,14), offset=Vec2(-18,15), step=Vec2(0.2,0.2), isSolid=false, sound=Sound.sting,},
  { tag = 25,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(9,14), offset=Vec2(-18,15), step=Vec2(0.2,0.2), isSolid=false, sound=Sound.sting,},
  { tag = 26,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(11,14),offset=Vec2(-18,15), step=Vec2(0.2,0.2), isSolid=false, sound=Sound.sting,},
  { tag = 27,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(13,14),offset=Vec2(-18,15), step=Vec2(0.2,0.2), isSolid=false, sound=Sound.sting,},
  { tag = 28,type = RoleType.npc,frame = "agnail_blue1.png",pos = Vec2(15,14),offset=Vec2(-18,15), step=Vec2(0.2,0.2), isSolid=false, sound=Sound.sting,}, 

  --问号

  { tag = 29,type = RoleType.robject,frame = "question_green.png",pos = Vec2(6,18),offset=Vec2(0,0), isSolid=false, onCheckTile=false,},
  { tag = 30,type = RoleType.robject,frame = "question_red.png",pos = Vec2(9,18),offset=Vec2(0,0), allowTouch=true, allowDragX=true,isSolid=false, onCheckTile=false,},
  { tag = 31,type = RoleType.robject,frame = "question_green.png",pos = Vec2(12,18),offset=Vec2(0,0), isSolid=false, onCheckTile=false,},
  --obb saw
  { tag = 32,type = RoleType.robject,file = Folder.texture.."saw.png",pos = Vec2(8,6),offset=Vec2(0,0), isSolid=false, onCheckTile=false,},
   --金币 left
  { tag = 33,type = RoleType.robject,frame = "diamond.png",pos = Vec2(11,4),offset=Vec2(0,0), triggerSaw=true, allowNotify=true,zorder=ZOrder.robject-1,onCheckTile=false,isSolid=false},
  { tag = 34,type = RoleType.robject,frame = "coin.png",pos = Vec2(11,5),offset=Vec2(0,0), allowNotify=true,zorder=ZOrder.robject-1, onCheckTile=false,isSolid=false},
  { tag = 35,type = RoleType.robject,frame = "coin.png",pos = Vec2(11,6),offset=Vec2(0,0), allowNotify=true,zorder=ZOrder.robject-1, onCheckTile=false,isSolid=false}, 
  { tag = 36,type = RoleType.robject,frame = "coin.png",pos = Vec2(11,7),offset=Vec2(0,0), allowNotify=true,zorder=ZOrder.robject-1, onCheckTile=false,isSolid=false}, 
  { tag = 37,type = RoleType.robject,frame = "coin.png",pos = Vec2(11,8),offset=Vec2(0,0), allowNotify=true,zorder=ZOrder.robject-1, onCheckTile=false,isSolid=false}, 
  { tag = 38,type = RoleType.robject,frame = "coin.png",pos = Vec2(11,9),offset=Vec2(0,0), allowNotify=true,zorder=ZOrder.robject-1, onCheckTile=false,isSolid=false},
 
  { tag = 39,type = RoleType.robject,frame = "diamond.png",pos = Vec2(12,4),offset=Vec2(0,0), triggerSaw=true,allowNotify=true,zorder=ZOrder.robject-1, onCheckTile=false,isSolid=false,},
  { tag = 40,type = RoleType.robject,frame = "coin.png",pos = Vec2(12,5),offset=Vec2(0,0), allowNotify=true,zorder=ZOrder.robject-1, onCheckTile=false,isSolid=false,},
  { tag = 41,type = RoleType.robject,frame = "coin.png",pos = Vec2(12,6),offset=Vec2(0,0), allowNotify=true,zorder=ZOrder.robject-1, onCheckTile=false,isSolid=false,}, 
  { tag = 42,type = RoleType.robject,frame = "coin.png",pos = Vec2(12,7),offset=Vec2(0,0), allowNotify=true,zorder=ZOrder.robject-1, onCheckTile=false,isSolid=false,}, 
  { tag = 43,type = RoleType.robject,frame = "coin.png",pos = Vec2(12,8),offset=Vec2(0,0), allowNotify=true,zorder=ZOrder.robject-1, onCheckTile=false,isSolid=false,}, 
  { tag = 44,type = RoleType.robject,frame = "coin.png",pos = Vec2(12,9),offset=Vec2(0,0), allowNotify=true,zorder=ZOrder.robject-1, onCheckTile=false,isSolid=false,},  
  -- flatboard
 { tag = 45,type = RoleType.robject,file=Folder.texture.."board_flat.png",pos = Vec2(10,29),offset=Vec2(0,0), isSolid=false, onCheckTile=false, hintIndex=1,resetBoard = function(index)
      Role[45].ref.isFlippedX = FlatHints[index].isFlippedX ~= nil
      if FlatHints[index].pos ~= nil then
        Role[45].ref:spawn(FlatHints[index].pos,FlatHints[index].offset)
      end
    end,
  },
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
    if (Joystick.ref:getTouchedPad(1)) then  
      Role[1].ref:getWeapon():shootAt(Vec2.ZERO)
    end
  end
end

preload = function()
  --根据语言载入对话,cpp动态写入语言，必须在函数内执行
  require("dialog".."6-"..Language)
  PopupInfo = require("popup"..Language)
  --预先载入公共事件或属性，比如Role里面的事件
  preloadStage()
  local iter = 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Player.Routine,nil) iter = iter + 1

  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,State6.Npc.AgnailArray1) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,State6.Npc.AgnailArray1) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,State6.Npc.AgnailArray1) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,State6.Npc.AgnailArray1) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,State6.Npc.AgnailArray1) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,State6.Npc.AgnailArray1) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,State6.Npc.AgnailArray1) iter = iter + 1
--agnail2
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,State6.Npc.AgnailArray2) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,State6.Npc.AgnailArray2) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,State6.Npc.AgnailArray2) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,State6.Npc.AgnailArray2) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,State6.Npc.AgnailArray2) iter = iter + 1
--question
  Role[iter].fsm = Fsm:new(Role[iter],nil,State6.RObject.QuestionTop) iter = iter + 1
 
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1

  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  --question array
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  --saw
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  -- coin left
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1 
  --
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State6.Npc.Routine,nil) iter = iter + 1 
  --coin bottom
 
  Role[iter].fsm = Fsm:new(Role[iter],nil,RObjectStates.FlatBoard) iter = iter + 1 
  iter = 1
  Triggers[iter].fsm = Fsm:new(Triggers[iter],nil,State6.Triggers.Spike)  iter = iter + 1
  Triggers[iter].fsm = Fsm:new(Triggers[iter],nil,nil)  iter = iter + 1
end
-- cpp初始化调用，
initiate = function()  
  Resh.playMusic(Music.BitShift)
  --调试
--   getStage():setPtc(Vec2(0,11),Vec2(0,-4))
--   Role[1].ref:spawn(Vec2(15,10),Vec2.ZERO) 
   --刷新数据，否则会自动存储上一次结果
  State6.refresh()
  if (Checkpoint.id  > 0) then  --saw
    --清除第一阶段fsm
    for i=2,RoleIndex.agnail13,1 do
      Role[i].fsm:clearState()
    end
    --销毁
    Role[RoleIndex.questionTop].isDisposed = true 

    if (Checkpoint.id == 1) then
      Role[RoleIndex.flatBoard].hintIndex = 2
      Role[RoleIndex.flatBoard].resetBoard(2)
      Role[RoleIndex.questionLeft].fsm:changeState(State6.RObject.QuestionArray)
      Role[RoleIndex.questionMiddle].fsm:changeState(State6.RObject.QuestionArray)
      Role[RoleIndex.questionRight].fsm:changeState(State6.RObject.QuestionArray)
      getStage():setPtc(Vec2(0,21),Vec2(0,-4))
      getStage():removeTileRange(Vec2(8,21),Vec2(9,22))
      Role[1].ref:spawn(Checkpoint.position,Vec2.ZERO) 
    else
      Role[RoleIndex.flatBoard].hintIndex = 4
      Role[RoleIndex.flatBoard].resetBoard(4)
      Role[RoleIndex.saw].ref:setAnimation( { type = Anistyle.rotate,anchor=Vec2(0,0.5), duration=8, angle=365,limitedTimes=-1,})
      Role[RoleIndex.saw].fsm:changeState(RObjectStates.RotateOBB)
      getStage():setPtc(Vec2(0,11),Vec2(0,-4))
      Role[1].ref:spawn(Checkpoint.position,Vec2.ZERO) 
    end
  end
  globalInit()
end
--循环调用

update = function() 
  if(checkRetry()) then
    updateGlobal()
  end
end