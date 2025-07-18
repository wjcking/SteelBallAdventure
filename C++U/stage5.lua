--测试状态机在里面
require("global")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local NpcStates = require("npcstates")

local Telegram = require("telegram")
local State5 = require("state5")
PopupInfo = nil
Resources =
{  
  {music = Music.BitShift},
}
Background =
{
  {file= Folder.background.."sky.png",},
}

-- 地图
Atlas =
{
  {file = "map/map5.tmx", view = MapView.panorama} 
}
Triggers =
{
  {type=0, name="spike1",  isActive=true}, 
  {type=0, name="spike2",  isActive=true}, 
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

}

Joypad =
{
  --{ file = "control/button.png",pos = Vec2(ScreenWidth - 85, 40), name = "attack1", text="",visible=false },
}

RoleIndex =
{
  btnRed =2,
  btnGreen=3,
  btnBlue=4,
  btnBottomRight = 5,
  btnTopLeft = 6,
  btnTopRight = 7,
  btnBottomLeft = 8, 
  block1=9,
  block2=10,
  wallBlock1=11,
  wallBlock2=12,
  wallBlock3=13,
  doubleSpikes=14,
  spikeTop = 15,
  spikeLeft1 = 16,
  spikeLeft2 = 17,
  buttonCenter=18,
  finalSpike=19,
  shard=20,
  flatBoard=21,
}
local FrameButtonRed =  {name="button_red", rangeStart=1,rangeEnd=2,direction=-3,sound = Sound.button,limitedTimes=1,  }
local FrameButtonGreen =  {name="button_green", rangeStart=1,rangeEnd=2,direction=-3,sound = Sound.button,limitedTimes=1, }
local FrameButtonBlue =  {name="button_blue", rangeStart=1,rangeEnd=2,direction=-3,sound = Sound.button,limitedTimes=1, }
local FrameButtonCorner =  {name="button_blue", rangeStart=1,rangeEnd=2,direction=-3,sound =Sound.button,limitedTimes=-1, }
Role = 
{
  { tag = 1,type = RoleType.player,frame = "player1.png",pos = Vec2(8,32),face = MovingDirection.toTop,insetTile = Vec2(13.0,3.0) },
  { tag = 2,type = RoleType.robject,frame = "button_red1.png", framePrefix="button_red", buttonIndex = 1,pos = Vec2(7,28), isSolid=false, onCheckTile=false, switch=FrameButtonRed},
  { tag = 3,type = RoleType.robject,frame = "button_green1.png",framePrefix="button_green",pos = Vec2(9,30), offset=Vec2(-20.5,0),isSolid=false, onCheckTile=false,switch=FrameButtonGreen},
  { tag = 4,type = RoleType.robject,frame = "button_blue1.png",framePrefix="button_blue",pos = Vec2(10,28),isSolid=false, onCheckTile=false,switch=FrameButtonBlue},
  --角落-- 第一个右下角
  { tag = 5,type = RoleType.robject,frame = "button_blue1.png",framePrefix="button_blue",pos = Vec2(16,33),isSolid=false, onCheckTile=false,switch=FrameButtonCorner,visible=false,},
  --左上角
  { tag = 6,type = RoleType.robject,frame = "button_blue1.png",framePrefix="button_blue",pos = Vec2(1,26),isSolid=false, onCheckTile=false,switch=FrameButtonCorner,visible=false,},
  --右上角
  { tag = 7,type = RoleType.robject,frame = "button_blue1.png",framePrefix="button_blue",pos = Vec2(16,26),isSolid=false,switch=FrameButtonCorner,visible=false,},
  --左下角
  { tag = 8,type = RoleType.robject,frame = "button_blue1.png",framePrefix="button_blue",pos = Vec2(1,33),isSolid=false,switch=FrameButtonCorner,visible=false,},
  { tag = 9,  type = RoleType.robject,  tile=TileFlat.brick, pos = Vec2(6,29), },
  { tag = 10, type = RoleType.robject, tile=TileFlat.brick, pos = Vec2(11,29), },
  --第二阶段 3个砖块

  { tag = 11, type = RoleType.robject, tile=TileFlat.blockWall, pos = Vec2(13,23), offset=Vec2(0,0), allowPush=CollisionDirection.atLeft},
  { tag = 12, type = RoleType.robject, tile=TileFlat.blockWall, pos = Vec2(16,21), offset=Vec2(0,0), allowPush=CollisionDirection.atLeft},
  { tag = 13, type = RoleType.robject, tile=TileFlat.blockWall, zorder=ZOrder.cover, pos = Vec2(16,23), offset=Vec2(0,0), allowPush=CollisionDirection.atLeft},
  --双星
  { tag = 14, type = RoleType.robject, file=Folder.texture.."doublespikes.png", pos = Vec2(2,16), offset=Vec2(0,22), onCheckTile=false, allowGravity=false, isHarmful = true, isSolid=false, sound=Sound.sting},
  --三个刺
  { tag = 15, type = RoleType.robject, frame="spiketop.png", pos = Vec2(5,22), offset=Vec2(0,11.5),onCheckTile=false, allowGravity=false, isHarmful = true, isSolid=false, sound=Sound.sting},
  { tag = 16, type = RoleType.robject, frame="spikeleft.png", pos = Vec2(12,23), offset=Vec2(12,0),onCheckTile=false, allowGravity=false, isHarmful = true, isSolid=false, sound=Sound.sting, allowFlip = false, step=Vec2(4,4)},
  { tag = 17, type = RoleType.robject, frame="spikeleft.png", pos = Vec2(12,18),offset=Vec2(12,0), onCheckTile=false, allowGravity=false, isHarmful = true, isSolid=false, sound=Sound.sting},
  --左下角
  { tag = 18,type = RoleType.robject,frame = "button_red1.png",framePrefix="button_red",pos = Vec2(9,20),isSolid=false,switch=FrameButtonRed,},
  { tag = 19,type = RoleType.robject,file=Folder.texture.."doublespikes.png",pos = Vec2(13,12),isSolid=false, onCheckTile=false,isHarmful = true,},
  { tag = 20,type = RoleType.robject,file=Folder.texture.."shardblue.png",pos = Vec2(14,12),offset=Vec2(20,0), isSolid=false, onCheckTile=false, animate=Animate.hoverCrystal,},
  { tag = 21,type = RoleType.robject,file=Folder.texture.."board_flat.png",pos = Vec2(12,31),offset=Vec2(0,0), isSolid=false, onCheckTile=false, hintIndex=1,resetBoard = function(index)
      Role[21].ref.isFlippedX = FlatHints[index].isFlippedX ~= nil
      if FlatHints[index].pos ~= nil then
        Role[21].ref:spawn(FlatHints[index].pos,FlatHints[index].offset)
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

  end
end

preload = function()
  --根据语言载入对话,cpp动态写入语言，必须在函数内执行
  require("dialog".."5-"..Language)
  PopupInfo = require("popup"..Language)
  --预先载入公共事件或属性，比如Role里面的事件
  preloadStage()
  local iter = 1
  Role[iter].fsm = Fsm:new(1,State5.Player.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State5.RObject.ButtonRGB) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State5.RObject.ButtonRGB) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State5.RObject.ButtonRGB) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1 
  --第二阶段
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil ) iter = iter + 1 
  Role[iter].fsm = Fsm:new(Role[iter],nil,State5.RObject.WallBlock ) iter = iter + 1 
  Role[iter].fsm = Fsm:new(Role[iter],nil,State5.RObject.WallBlock ) iter = iter + 1 
  Role[iter].fsm = Fsm:new(Role[iter],nil,State5.RObject.DoubleSpikes) iter = iter + 1 

  Role[iter].fsm = Fsm:new(Role[iter],nil,State5.Triggers.Spikes) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State5.Triggers.Spikes) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State5.Triggers.Spikes) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State5.RObject.ButtonCenter) iter = iter + 1 
  --第三阶段
  Role[iter].fsm = Fsm:new(Role[iter],nil,State5.RObject.FinalSpike) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,RObjectStates.Destiny) iter = iter + 1 
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1 
  iter = 1
  Triggers[iter].fsm = Fsm:new(Triggers[iter],nil,State5.Triggers.Spike)  iter = iter + 1
  Triggers[iter].fsm = Fsm:new(Triggers[iter],nil,nil)  iter = iter + 1
end
-- cpp初始化调用，
initiate = function()
  Ref.stage:loadCurtain({type = CurtainType.black,   delay = 4, zorder = ZOrder.robject})
  Resh.playMusic(Music.BitShift)
  State5.refresh()
  --测试直接到位
--   getStage():setPtc(Vec2(0,14),Vec2(0,0))
--  Role[1].ref:spawn(Vec2(3,13),Vec2.ZERO)
  --地图位置
  --checkpoint
  if (Checkpoint.id > 0 ) then
    if (Checkpoint.id == 1) then
      Role[RoleIndex.flatBoard].hintIndex = 3
      Role[RoleIndex.flatBoard].resetBoard(3)
      getStage():setPtc(Vec2(0,Checkpoint.position.y),Vec2(0,-4))
    else
      Role[RoleIndex.flatBoard].hintIndex = 4
      getStage():setPtc(Vec2(0,14),Vec2(0,0))
      Role[RoleIndex.flatBoard].resetBoard(4)
    end
    Role[1].ref:spawn(Checkpoint.position,Vec2.ZERO)
  end
  Role[RoleIndex.flatBoard].fsm:changeState(RObjectStates.FlatBoard)
  createStar(4,Vec2(11,9),ZOrder.cover)
  globalInit()
end
--循环调用

update = function() 
  if(checkRetry()) then
    updateGlobal()
  end
end