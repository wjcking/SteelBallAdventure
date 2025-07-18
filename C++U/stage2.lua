--测试状态机在里面
require("global")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local NpcStates = require("npcstates")
local CommonStates = require("commonstates")
local Telegram = require("telegram")
local State2 = require("state2")
PopupInfo = nil
Resources =
{  
  {music = Music.paraspritePolka},
}
Background =
{
  {file= Folder.background.."sky.png",},
}
-- 地图
Atlas =
{
  {file = "map/map2.tmx" , view =  MapView.horizontal, image=Folder.background.."wood.png" } 
}
Triggers =
{
  {type=0, name="spike",  isActive=true}, 
  {type=0, name="collapse1",  isActive=true}, 
}
--对话范围 cpp下标 0 开始
Segment =
{
  { rangeStart = 0,  rangeEnd= 5, },
}

--如果lua中上一次的collapse存在，务必重置后才能使用否则会出现range越界,定义变量不赋值则可以清空
Collapse =
{

}

CommandPad = 
{

}
Joystick =
{
  type = 2,
  controlTag = 1,
}
Joypad =
{
  { file = "control/jump.png",pos = Vec2(ScreenWidth - 70, 45), name = "jump" },
  ---{ file = "control/fire.png",pos = Vec2(ScreenWidth - 70 * 2.5, 45), name = "fire" },
}

RoleIndex =
{
  dialogBoard = 4,
  fruitTrace = 8,
  zzi = 13,
  block1=11,
  block2=12,
  apple1=5,
  appleLast=11,
}
local knocks =
{
  { type=KnockAction.hidden, direction=CollisionDirection.atBottom,tile=Tile.grayBrick, sound= Sound.coin,toast=Animate.toastCoin},
}
Role = 
{
  { tag = 1,type = RoleType.player,frame = "player1.png",pos = Vec2(6,17),hp=2,hpMax=2,},
  { tag = 2,type = RoleType.robject, tile=Tile.grayBlock,pos = Vec2(3, 15)  },
  { tag = 3,type = RoleType.robject,frame = "agnail_red_glasses1.png", sound=Sound.sting,pos = Vec2(3,15), isSolid=false,isHarmful=true,zorder=1,onCheckTile=false, allowGravity=false},
  { tag = 4,type = RoleType.robject,frame = "dialog_info.png",pos = Vec2(7,13),   text = RObjectText.notice, replacedText=RObjectText.board, allowCollision=CollisionDirection.atTop, allowTouch=true,touchedText = RObjectText.touched},
  { tag = 5,type = RoleType.robject,frame = "fruit_green.png",pos = Vec2(19,13), offset=Vec2(0,0),isHarmful = true,  onCheckTile=false,isSolid=false, isHarmful=false,},
  { tag = 6,type = RoleType.robject,frame = "fruit_red.png",pos = Vec2(23,13), offset=Vec2(-25,0),   onCheckTile=false,isSolid=false, animate=Animate.jumpShake},
  { tag = 7,type = RoleType.robject,frame = "fruit_green.png",pos = Vec2(24,13), offset=Vec2(5,-25),   onCheckTile=true, isSolid=false, isHarmful = true},
  { tag = 8,type = RoleType.robject,frame = "fruit_green.png",pos = Vec2(28,12), offset=Vec2(0,0),   onCheckTile=false,isSolid=false,isHarmful=true,allowGravity=false, step=Vec2(1,1)},
  { tag = 9,type = RoleType.robject,frame = "question_green.png",pos = Vec2(28,17), offset=Vec2(0,0),allowGravity=false,onCheckTile=false,isSolid=false,step=Vec2(2.2,2.2)},
  { tag = 10,type = RoleType.robject,frame = "fruit_green.png",pos = Vec2(33,14), offset=Vec2(20,0),allowGravity=false,onCheckTile=false,isSolid=false,step=Vec2(2.5,2.5)},
  { tag = 11,type = RoleType.robject,frame = "fruit_green.png",pos = Vec2(35,13), offset=Vec2(-10,-10), onCheckTile=false,  isSolid=false,isHarmful=false,},
  { tag = 12,type = RoleType.robject,tile = Tile.grayBlock,pos = Vec2(39,15),offset=Vec2(-6,2), onCheckTile=false, knocks=knocks},
  { tag = 13,type = RoleType.npc,file = Folder.texture.."zzi_cloud.png",pos = Vec2(56,15),offset=Vec2(0,17), step=Vec2(1,1),face = MovingDirection.toRight, allowGravity= false, onCheckTile=false, animate={ type = Anistyle.rebound,delay=0 ,offset= Vec2(0,10)},},
  { tag = 14,type = RoleType.npc,frame = "cloud11.png", framePrefix="cloud1",pos = Vec2(40,12), sound=Sound.sting, isSolid = false,onCheckTile=false,allowGravity=false,},
}


--回调事件，funcName函数名
callback = function(className,funcName,params) 
  --全局回调函数
  globalCallFunc(className,funcName,params)
  if (className == "gun" and funcName =="shoot") then
    RoleModel.addProjectObject(params.ref,RObjectStates.ProjectTile)
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
--    if (Joystick.ref:getTouchedPad(2)) then 
--      Role[1].ref:getWeapon():shootAt(Vec2(0,0));
--    end
  end
end

preload = function()
  --根据语言载入对话,cpp动态写入语言，必须在函数内执行
  require("dialog".."2-"..Language)
  PopupInfo = require("popup"..Language)
  --预先载入公共事件或属性，比如Role里面的事件
  preloadStage()

  local iter = 1
  Role[iter].fsm = Fsm:new(Role[iter],State2.Player.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State2.Npc.Agnail) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State2.RObject.DialogBoard) iter = iter + 1

  Role[iter].fsm = Fsm:new(Role[iter],nil,State2.RObject.Fall) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State2.RObject.FallAndSneakAttack) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State2.RObject.QuestionGreen) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State2.RObject.SneakTrace) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State2.RObject.Fall) iter = iter + 1

  Role[iter].fsm = Fsm:new(Role[iter],nil,State2.RObject.Knocked) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State2.RObject.Cloud) iter = iter + 1

  iter = 1
  Triggers[iter].fsm = Fsm:new(Triggers[iter],nil,State2.Trigger.Spike)  iter = iter + 1
  Triggers[iter].fsm = Fsm:new(Triggers[iter],nil,nil)  iter = iter + 1
end
-- cpp初始化调用，
initiate = function()
 
  Ref.stage:loadCurtain({type = CurtainType.updown, image="background/curtaintop.png",text="", delay=0.5 })
  Resh.playMusic(Music.paraspritePolka)
  Role[6].ref:setAnimation(Animate.jumpShake)
  --checkpoint
  if (Checkpoint.id > 0 ) then
    Role[1].ref:spawn(Checkpoint.position,Vec2.ZERO)
  end

  Role[RoleIndex.zzi].ref.isFlippedX = true
  createStar(1,Vec2(Role[RoleIndex.dialogBoard].pos.x+6,11),ZOrder.robject)
  globalInit()
end
--循环调用
update = function()
  -- Role[1].ref.onJump = Joystick.ref:getTouchedPad(1)
 
  if (checkRetry()) then
    updateGlobal()
  end
end