--测试状态机在里面
require("global")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local NpcStates = require("npcstates")
local Telegram = require("telegram")
local CommonStates = require("commonstates")
local State10= require("state10")
PopupInfo = nil
Resources =
{  
  {music = Music.SaltyDitty},
}
Background =
{
  {file= Folder.background.."sky.png",},
}
-- 地图
Atlas =
{
  {file = "map/map10.tmx" , view =  MapView.horizontal, image=Folder.background.."stage10.png" }  
}
Triggers =
{
  {type=0, name="trap",  isActive=true}, 
}

--对话范围 cpp下标 0 开始
Segment =
{

}


--如果lua中上一次的collapse存在，务必重置后才能使用否则会出现range越界,定义变量不赋值则可以清空
Collapse =
{

}
CommandPad = 
{
  jump = "jump",

}
Joystick =
{
  type = 2,
  controlTag = 1,
}
Joypad =
{
  { file = "control/jump.png",pos = Vec2(ScreenWidth - 70, 45), name = CommandPad.jump }, 
}

RoleIndex =
{
  bug = 3,
  keyBlue = 5,
  keyRed = 6,
  drink=8,
  board=13,
  pipe=15,
  reboundAgnail=16,
}
local knocks =
{ type=KnockAction.hidden, direction=CollisionDirection.atBottom,tile=Tile.grayBrick, sound= Sound.coin,toast=Animate.toastCoin 
}

Role = 
{
  { tag = 1,type = RoleType.player,frame = "player1.png",pos = Vec2(7,17),},
  { tag = 2,type = RoleType.npc, frame="bird_gray1.png", step=Vec2(0.3,0.3),direction=MovingDirection.toRight, face=MovingDirection.toLeft,framePrefix="bird_gray",pos = Vec2(3,14), ac=CollisionDirection.atTop, isSolid=true, isHarmful=false,frameIndexes=FrameTemplate.bird, allowGravity=false, wanderX=ScreenWidth/2.8, },
  { tag = 3,type = RoleType.npc, frame="bug_blue1.png", pos=Vec2(1,17), step=Vec2(1,1), insetY=4,},
  { tag = 4,type = RoleType.robject, tile=Tile.spikeTop, pos=Vec2(13,14),sound=Sound.sting, isSolid=false,onCheckTile=false, isHarmful=true, allowCheck=true,},
  { tag = 5,type = RoleType.robject,frame="key_blue.png", pos=Vec2(1,11), offset=Vec2(0,-5),allowNotify=true,isSolid=false,onCheckTile=false,  },
  { tag = 6,type = RoleType.robject,frame="key_red.png", pos=Vec2(13,12), offset=Vec2(0,0),allowNotify=true,isSolid=false,onCheckTile=false,  },
  { tag = 7,type = RoleType.npc,frame="agnail_red1.png", pos=Vec2(28,17),sound=Sound.sting, allowNotify=true,isSolid=false,onCheckTile=true, allowGravity=true, step=Vec2(0.15,0.15),},
  { tag = 8,type = RoleType.robject,frame="drink1.png", pos=Vec2(36,17), sound=Sound.manDied,  offset=Vec2(0,2), allowNotify=true,isSolid=false,onCheckTile=false, isHarmful=true, },

  { tag = 9,type = RoleType.robject,tile = Tile.grayBlock,pos = Vec2(33,13),  onCheckTile=false, knocks=knocks},
  { tag = 10,type = RoleType.robject,tile = Tile.grayBlock,pos = Vec2(33,15),  onCheckTile=false, knocks=knocks},
  { tag = 11,type = RoleType.robject,tile = Tile.grayBlock,pos = Vec2(34,15),  onCheckTile=false, knocks=knocks},
  { tag = 12,type = RoleType.robject,tile = Tile.grayBlock,pos = Vec2(35,15),  onCheckTile=false, knocks=knocks},

  { tag = 13,type = RoleType.robject, frame="dialog_info.png", allowTouch=true, pos=Vec2(40,13), onCheckTile=false, isSolid=false, text=RObjectText.dialogInfo},
  { tag = 14,type = RoleType.robject, file=Folder.texture.."8spikes.png", offset=Vec2(-24,0), pos=Vec2(48,11), onCheckTile=false, isSolid=false, sound=Sound.sting,isHarmful=true, },  
  { tag = 15,type = RoleType.robject, file=Folder.texture.."pipe.png", offset=Vec2(30,-4), pos=Vec2(59,18), onCheckTile=false, isSolid=true, allowTouch=true,allowDragY=true,zorder=ZOrder.cover, }, 
      { tag = 16,type = RoleType.npc, frame="agnail_gray3.png", offset=Vec2(0,0), pos=Vec2(67,13), onCheckTile=false,zorder=ZOrder.cover, allowGravity=false, sound=Sound.sting }, 
   { tag = 17,type = RoleType.robject, file=Folder.texture.."shardorange.png", offset=Vec2(0,25), pos=Vec2(69,17), onCheckTile=false, isSolid=false, allowTouch=true,allowDragY=true,animate=Animate.hoverCrystal }, 
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
  if className == "stage"  then
 
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
  require("dialog".."10-"..Language)
  PopupInfo = require("popup"..Language)
  --预先载入公共事件或属性，比如Role里面的事件
  preloadStage()

  local iter = 1

  Role[iter].fsm = Fsm:new(Role[iter],State10.Player.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,NpcStates.WanderingX) iter = iter + 1 -- --
  Role[iter].fsm = Fsm:new(Role[iter],nil,State10.Npc.Bug) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State10.RObject.SpikeTop) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State10.RObject.Key) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State10.RObject.Key) iter = iter + 1
  --phase2
  Role[iter].fsm = Fsm:new(Role[iter],nil,State10.Npc.Agnail) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State10.RObject.Drink) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,RObjectStates.Knocked) iter = iter + 1

  Role[iter].fsm = Fsm:new(Role[iter],nil,RObjectStates.Knocked) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,RObjectStates.Knocked) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,RObjectStates.Knocked) iter = iter + 1
  --board
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  --8spikes
  Role[iter].fsm = Fsm:new(Role[iter],nil,State10.RObject.EightSpikes) iter = iter + 1
  
  Role[iter].fsm = Fsm:new(Role[iter],nil,State10.RObject.Pipe) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State10.Triggers.Trap) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State10.RObject.ShardOrange) iter = iter + 1
  
  --触发器
  iter = 1 
  Triggers[iter].fsm = Fsm:new(Triggers[iter],nil,State10.Triggers.Trap) iter = iter + 1
end
-- cpp初始化调用，
initiate = function()  
 
      Ref.stage:loadCurtain({type = CurtainType.bilateral, image={"background/curtainleft.png","background/curtainright.png"},  delay = 1, zorder = ZOrder.dropCurtain})
  Resh.playMusic(Music.SaltyDitty)
  if (Checkpoint.id  > 0 ) then
    Role[1].ref:spawn(Checkpoint.position,Vec2.ZERO)
  end
  createStar(6,Vec2(51,17),ZOrder.cover) 
 globalInit()
end
--循环调用
update = function()

  if(checkRetry()) then
    updateGlobal()
  end

end