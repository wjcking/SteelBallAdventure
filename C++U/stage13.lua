--测试状态机在里面
require("global")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local NpcStates = require("npcstates")
local Telegram = require("telegram")
local CommonStates = require("commonstates")
local State13= require("state13")
LayerCover = "cover"
PopupInfo = nil
Resources =
{  
  { music = Music.Retro }, 
  { sound = Sound.teleport }, 
}
Background =
{
  {file= Folder.background.."night.png",},
}
-- 地图
Atlas =
{
  {file = "map/map13.tmx" , view = MapView.panorama,   }  
}
Triggers =
{
  {type=0,name="trap", }
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
}
Joystick = { type = 1, controlTag = 1, }
Joypad =
{

}

RoleIndex =
{
  buttonRed=6,
  buttonGreen=7,
  buttonBlue=8,
  spiral2 = 3,
  spiral3 = 4,
  spikeLeft1 = 9,
  spikeLeft2 = 10,
  wallWhite = 11,
  doubleSpikes = 12,
  spiralBottomLeft=13,
  spiralBottomRight=14,
  keyLeft =  16,
  keyRight =  17,
  crystal = 18,
  agnail = 15,
  flatBoard = 19,

}
local FrameButtonRed =  {name="button_red", rangeStart=1,rangeEnd=2,direction=-3,sound = Sound.button,limitedTimes=-1,  }
local FrameButtonGreen =  {name="button_green", rangeStart=1,rangeEnd=2,direction=-3,sound = Sound.button,limitedTimes=-1, }
local FrameButtonBlue =  {name="button_blue", rangeStart=1,rangeEnd=2,direction=-3,sound = Sound.button,limitedTimes=-1, }


Role = 
{
  { tag = 1,type = RoleType.player,frame = "player1.png",pos = Vec2(3,21),face = MovingDirection.toTop,insetTile = Vec2(13.0,3.0),},
  {tag=2,type = RoleType.robject,frame = "spiral_small.png", isSolid=false,  pos = Vec2(6,19),offset=Vec2(0,0), onCheckTile=false,isSolid=false,insetObject=Vec2(11,11), animate=Animate.rotateSelf,},
  {tag=3,type = RoleType.robject,frame = "spiral_small.png", isSolid=false,  pos = Vec2(6,15),offset=Vec2(0,0), onCheckTile=false,isSolid=false,insetObject=Vec2(11,11), animate=Animate.rotateSelf,},
  {tag=4,type = RoleType.robject,frame = "spiral_small.png", isSolid=false,  pos = Vec2(16,15),offset=Vec2(0,0), onCheckTile=false,isSolid=false,insetObject=Vec2(11,11), animate=Animate.rotateSelf,},
  {tag=5,type = RoleType.robject,frame = "spiral_small.png", isSolid=false,  pos = Vec2(16,19),offset=Vec2(0,0), onCheckTile=false,isSolid=false,insetObject=Vec2(11,11), animate=Animate.rotateSelf,},
  { tag = 6,type = RoleType.robject,frame = "button_red1.png", framePrefix="button_red",  pos = Vec2(6,22), isSolid=false, onCheckTile=false, switch=FrameButtonRed,switchResult=FrameSwitch(),},
  { tag = 7,type = RoleType.robject,frame = "button_green1.png",framePrefix="button_green",pos = Vec2(11,15), offset=Vec2(0,0),isSolid=false, onCheckTile=false,switch=FrameButtonGreen,switchResult=FrameSwitch(),},
  { tag = 8,type = RoleType.robject,frame = "button_blue1.png",framePrefix="button_blue",pos = Vec2(11,20),isSolid=false, onCheckTile=false,switch=FrameButtonBlue,switchResult=FrameSwitch(),},
  { tag = 9,type = RoleType.robject,file = Folder.texture.."spikeleft3.png", pos = Vec2(1,21), offset=Vec2(-11.5,0),isSolid=false, onCheckTile=false, isHarmful=true,sound=Sound.sting, step=Vec2(6.1,6.1), distance=Vec2(ScreenWidth/3.4,ScreenHeight),direction=MovingDirection.toRight,},
  { tag = 10,type = RoleType.robject,file = Folder.texture.."spikeleft3.png", pos = Vec2(1,16), offset=Vec2(-11.5,0),isSolid=false, onCheckTile=false, isHarmful=true,sound=Sound.sting, step=Vec2(0.3,0.3), distance=Vec2(ScreenWidth/3.4,ScreenHeight),direction=MovingDirection.toRight,},  
  { tag = 11,type = RoleType.robject,tile=TileFlat.wallWhite, pos = Vec2(4,19),  isSolid=true, allowPush=CollisionDirection.intersected,  },
  { tag = 12,type = RoleType.npc,file = Folder.texture.."doublespikes_h.png", pos = Vec2(8,23), offset=Vec2(25,0),isSolid=false, onCheckTile=false, visible=false,isHarmful=false, step=Vec2(1.4,1.4), distance=Vec2(ScreenWidth ,ScreenHeight * 2),direction=MovingDirection.toTop, zorder=2,},
  --第二阶段
  {tag=13,type = RoleType.robject,frame = "spiral.png", isSolid=false,  pos = Vec2(2,26),offset=Vec2(0,0), onCheckTile=false,isSolid=false,insetObject=Vec2(12,12), animate=Animate.rotateSelf,},
  {tag=14,type = RoleType.robject,frame = "spiral.png", isSolid=false,  pos = Vec2(15,26),offset=Vec2(0,0), onCheckTile=false,isSolid=false,insetObject=Vec2(12,12), animate=Animate.rotateSelf,},
  {tag=15,type = RoleType.npc,frame = "agnail_red1.png", isSolid=false,  pos = Vec2(4,6),offset=Vec2(0,-10), onCheckTile=true,isSolid=false, zorder=2, step=Vec2(6,6), sound=Sound.sting},
  {tag=16,type = RoleType.robject,frame = "key_green.png", isSolid=false,  pos = Vec2(4,5),offset=Vec2(0,0), onCheckTile=false,isSolid=false,  zorder=2,},
  {tag=17,type = RoleType.robject,frame = "key_red.png", isSolid=false,  pos = Vec2(16,5),offset=Vec2(0,0), onCheckTile=false,isSolid=false,  zorder=2,},
  {tag=18,type = RoleType.robject,frame = "shardgreen.png", isSolid=false,  pos = Vec2(8,6),offset=Vec2(20,20), onCheckTile=false,isSolid=false,  animate=Animate.hoverCrystal,isHarmful=true,sound=Sound.sting},
  --flatboard
    { tag = 19,type = RoleType.robject,file=Folder.texture.."board_flat.png",pos = Vec2(11,22),offset=Vec2(0,15), isSolid=false, onCheckTile=false, hintIndex=1,resetBoard = function(index)
      Role[19].ref.isFlippedX = FlatHints[index].isFlippedX ~= nil
      if FlatHints[index].pos ~= nil then
        Role[19].ref:spawn(FlatHints[index].pos,FlatHints[index].offset)
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
    Role[1].ref.onJump = Joystick.ref:getTouchedPad(1)

    if (Joystick.ref:getTouchedPad(2)) then  
      Role[1].ref:getWeapon():shootAt(Vec2(0,0))
    end
  end
end
preload = function()
  --根据语言载入对话,cpp动态写入语言，必须在函数内执行
  require("dialog".."13-"..Language)
  PopupInfo = require("popup"..Language)
  --预先载入公共事件或属性，比如Role里面的事件
  preloadStage()

  local iter = 1
  Role[iter].fsm = Fsm:new(Role[iter],State13.Player.Routine,nil) iter = iter + 1

  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1

  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1


  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  --第二阶段
  --spiral
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  --agnail left key green
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1

  --key right crystall
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
--flatboard
   Role[iter].fsm = Fsm:new(Role[iter],nil,RObjectStates.FlatBoard) iter = iter + 1
  --触发器
  iter = 1 

  Triggers[iter].fsm = Fsm:new(Triggers[iter],nil,nil) iter = iter + 1
end
-- cpp初始化调用，
initiate = function() 
  Ref.stage:loadCurtain({type = CurtainType.bilateral, image={"background/leftrage.png","background/rightdick.png"},  delay = 1, zorder = ZOrder.dropCurtain})
  Resh.playMusic(Music.Retro)

  --测试  
--  Checkpoint.id = 1
--  Checkpoint.position = Vec2(8,11)
  if (Checkpoint.id == 0) then
    Role[2].fsm:changeState(State13.RObject.Teleport)
    Role[3].fsm:changeState(State13.RObject.Teleport)
    Role[4].fsm:changeState(State13.RObject.Teleport)
    Role[5].fsm:changeState(State13.RObject.Teleport)

    Role[RoleIndex.buttonRed].fsm:changeState(State13.RObject.Button)
    Role[RoleIndex.buttonGreen].fsm:changeState(State13.RObject.Button)
    Role[RoleIndex.buttonBlue].fsm:changeState(State13.RObject.Button)

    Role[RoleIndex.wallWhite].fsm:changeState(State13.RObject.WallWhite)
    getStage():setPtc(Vec2(0,23),CameraSlideOffset)
    Role[1].ref:spawn(Vec2(3,21),Vec2.ZERO)
  else
    --陷阱加载
    Triggers[1].fsm:changeState(State13.Triggers.Trap)
    Role[RoleIndex.crystal].fsm:changeState(State13.RObject.CrystalShard) 
    if (Checkpoint.id == 1) then
      Role[RoleIndex.doubleSpikes].isDisposed = true
      getStage():setPtc(Vec2(0,33),CameraSlideOffset)
      Role[1].ref:spawn(Checkpoint.position,Vec2.ZERO)
      Role[RoleIndex.spiralBottomLeft].fsm:changeState(State13.RObject.Teleport)
      Role[RoleIndex.spiralBottomRight].fsm:changeState(State13.RObject.Teleport)
      Role[RoleIndex.keyLeft].fsm:changeState(State13.RObject.Key)
      Role[RoleIndex.flatBoard].hintIndex = 2
      Role[RoleIndex.flatBoard].resetBoard(2)
    end 
    if (Checkpoint.id == 2) then
      Role[RoleIndex.doubleSpikes].isDisposed = true
      Role[RoleIndex.keyLeft].isDisposed = true
      getStage():setPtc(Vec2(0,13),CameraSlideOffset)
      Role[1].ref:spawn(Checkpoint.position,Vec2.ZERO)
      Role[RoleIndex.spiralBottomLeft].fsm:changeState(State13.RObject.Teleport)
      Role[RoleIndex.spiralBottomRight].fsm:changeState(State13.RObject.Teleport)
    Role[RoleIndex.flatBoard].hintIndex = 4
      Role[RoleIndex.flatBoard].resetBoard(4)
      getStage():removeLayerRange(LayerCover,Vec2(13,5),Vec2(16,12))
    end 
  end
  
  createStar(9,Vec2(1,32),ZOrder.cover)
  globalInit()
end
--循环调用
update = function()

  if(checkRetry(2.5)) then
    updateGlobal()
  end
end