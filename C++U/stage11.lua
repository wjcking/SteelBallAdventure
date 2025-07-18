--测试状态机在里面
require("global")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local NpcStates = require("npcstates")
local Telegram = require("telegram")
local CommonStates = require("commonstates")
local State11= require("state11")
PopupInfo = nil
Resources =
{  
  {music = Music.Yoyo},
  {sound = Sound.spring},
}
Background =
{
  {file= Folder.background.."sky.png",},
}
-- 地图
Atlas =
{
  {file = "map/map11.tmx" , view =  MapView.horizontal,  image=Folder.background.."stage11.png", }
}
Triggers =
{
  {type=0,name="trap",}
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
  cloudDialog=4,
  float1 = 5,
  float2 =6,
  float3 = 7,
  float4 = 8,
  float5 = 10,
  spring1 = 11,
  spring2 = 12,
  agnail2 = 15,
  agnail3= 16,
  agnail4= 17,
  doubleSpikes=18,
}
local knocks =
{
  type=KnockAction.hidden, direction=CollisionDirection.atBottom,tile=Tile.grayBrick, sound= Sound.coin,toast=Animate.toastCoin 
}
local rebound =
{
  type = Anistyle.rebound,limitedTimes=-1, offset=Vec2(0,ScreenHeight*0.7), duration=2, durationBack=2, delay=0,delayBack=0 
}
--只适用于checkFollow 
local moves = 
{ 
  {step=Vec2(11,11),direction=MovingDirection.toBottom, distance=Vec2(250,250)}, 
  {step=Vec2(11,11),direction=MovingDirection.toTop, distance=Vec2(250,250)},
}
local spring = {jumpForce=Vec2(0,5)}
Role = 
{
  { tag = 1,type = RoleType.player, frame = "player1.png",  pos = Vec2(2,15),},
  { tag = 2,type = RoleType.npc,frame = "agnail_gray1.png",pos = Vec2(0,15), offset=Vec2(-50,0),   sound=Sound.sting, direction=MovingDirection.toRight, step=Vec2(0.7,0),  }, 
  { tag = 3,type = RoleType.robject,frame = "cloud11.png",pos = Vec2(5,12), offset=Vec2(20,0), onCheckTile=false, isSolid = false, allowNotify=true, sound=Sound.sting, isHarmful=true, }, 
  { tag = 4,type = RoleType.robject,file = Folder.texture.."dialog_cloud.png",pos = Vec2(13,12), offset=Vec2(20,0), onCheckTile=false, isSolid = false, text=RObjectText.dialogCloud, allowTouch=true,},
  { tag = 5,type = RoleType.robject,frame = "float_yellow.png",pos = Vec2(6,17),  onCheckTile=false, allowCollision=CollisionDirection.atTop, allowNotify = true,  direction=MovingDirection.toTop,step=Vec2(0.7,0.7),},
  { tag = 6,type = RoleType.robject,frame = "float_yellow.png",pos = Vec2(10,15), onCheckTile=false, allowCollision=CollisionDirection.atTop, allowNotify = true,gravity=Vec2(-0.02,-0.02),},
  { tag = 7,type = RoleType.robject,frame = "float_yellow.png",pos = Vec2(15,17),  onCheckTile=false, allowCollision=CollisionDirection.atTop, allowNotify = true,},
  { tag = 8,type = RoleType.robject,frame = "float_yellow.png",pos = Vec2(18,17), offset=Vec2(15,0), onCheckTile=false, allowCollision=CollisionDirection.atTop, allowNotify = true, direction=MovingDirection.toLeft,step=Vec2(6,0),},
  { tag = 9,type = RoleType.robject, tile=Tile.grayBlock, pos = Vec2(22,14), offset=Vec2(4,-12), onCheckTile=false, knocks=knocks,},
  { tag = 10,type = RoleType.robject,frame = "float_yellow.png",pos = Vec2(26,17),  onCheckTile=false, allowCollision=CollisionDirection.atTop, allowNotify = true, direction=MovingDirection.toTop},
-- phase 2 注意弹簧step设置太低，弹跳不起来
  { tag = 11,type = RoleType.robject,frame = "spring_red1.png",pos = Vec2(40,18),  onCheckTile=false, allowCollision=CollisionDirection.atTop, allowNotify = true, insetY=4, step=Vec2(0.51,0.51), bounceTimes = 0},
  { tag = 12,type = RoleType.robject,frame = "spring_gray1.png",pos = Vec2(41,18), offset=Vec2(10,0), onCheckTile=false, allowCollision=CollisionDirection.atTop, allowNotify = true, insetY=4, step=Vec2(0.51,0.51), bounceTimes = 0,spring=spring,},
  { tag = 13,type = RoleType.robject,frame="dialog_info.png",pos = Vec2(42,12), offset=Vec2(-20,0), onCheckTile=false, isSolid = false, text=RObjectText.dialogSpring, allowTouch=false,},
  { tag = 14,type = RoleType.npc,frame = "4agnails_red1.png",pos = Vec2(45,16), offset=Vec2(0,0),  allowGravity=false, isSolid=false,onCheckTile=false, sound=Sound.sting,  }, 
  { tag = 15,type = RoleType.npc,frame = "4agnails_red1.png",pos = Vec2(50,16), offset=Vec2(0,0),  allowGravity=false, isSolid=false,onCheckTile=false, sound=Sound.sting,  }, 
  { tag = 16,type = RoleType.npc,frame = "4agnails_red1.png",pos = Vec2(55,16), offset=Vec2(0,0),  allowGravity=false, isSolid=false,onCheckTile=false, sound=Sound.sting, direction=MovingDirection.toRight, step=Vec2(2.5,2.5)}, 
  { tag = 17,type = RoleType.npc,frame = "4agnails_red1.png",pos = Vec2(64,12), offset=Vec2(0,0),  allowGravity=false, isSolid=false,onCheckTile=false, sound=Sound.sting,direction=MovingDirection.toRight, step=Vec2(3.0,3.0),}, 
  { tag = 18,type = RoleType.npc,file = Folder.texture.."doublespikes_h.png",pos = Vec2(90,12), offset=Vec2(15,0),  allowGravity=false, isSolid=false,onCheckTile=false, visible=false, sound=Sound.sting,gravity=Vec2(-0.11,-0.11), isHarmful=false,}, 
  { tag = 19,type = RoleType.robject,file = Folder.texture.."shardcyan.png",pos = Vec2(106,12), offset=Vec2(0,15),  allowGravity=false, isSolid=false,onCheckTile=false, animate=Animate.hoverCrystal}, 
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
  require("dialog".."11-"..Language)
  PopupInfo = require("popup"..Language)
  --预先载入公共事件或属性，比如Role里面的事件
  preloadStage()

  local iter = 1
  Role[iter].fsm = Fsm:new(Role[iter],State11.Player.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,CommonStates.Respawn) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,RObjectStates.Cloud) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State11.RObject.Float) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State11.RObject.Float) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State11.RObject.Float) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State11.RObject.Float) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,RObjectStates.Knocked) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State11.RObject.Float) iter = iter + 1
  --phase 2
  Role[iter].fsm = Fsm:new(Role[iter],nil,State11.RObject.Spring) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State11.RObject.Spring) iter = iter + 1

  Role[iter].fsm = Fsm:new(Role[iter],nil,State11.RObject.DialogSpring) iter = iter + 1

  Role[iter].fsm = Fsm:new(Role[iter],nil,State11.Npc.Agnails) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State11.Npc.Agnails) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State11.Npc.Agnails) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State11.Npc.Agnails) iter = iter + 1
  --doublespikes
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
     
  Role[iter].fsm = Fsm:new(Role[iter],nil,RObjectStates.Destiny) iter = iter + 1
  --触发器
  iter = 1 

  Triggers[iter].fsm = Fsm:new(Triggers[iter],nil,State11.Triggers.Trap) iter = iter + 1
end
-- cpp初始化调用，
initiate = function()  
  --  Role[RoleIndex.hover1].ref:registerMoves(moves)
  if (Checkpoint.id == 1) then
   Role[1].ref:spawn(Checkpoint.position,Vec2.ZERO)
 
   --  Role[RoleIndex.hover1].fsm:changeState(RObjectStates.Move)
  end

  --Role[1].ref:spawn(Vec2(36,14),Vec2.ZERO)
  Resh.playMusic(Music.Yoyo)
   createStar(7,Vec2(91,14),ZOrder.robject)
   globalInit()
end
--循环调用
update = function()

  if(checkRetry()) then
    updateGlobal(roleID)
  end

end