--测试状态机在里面
require("global")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local NpcStates = require("npcstates")
local Telegram = require("telegram")
local CommonStates = require("commonstates")
local State8= require("state8")
PopupInfo = nil
Resources =
{  
  {sound=Sound.balloonBroken},
  {sound=Sound.poogun},
  {music=Music.BubbleBobble2},
}
Background =
{
  {file= Folder.background.."sky.png",},
}
-- 地图
Atlas =
{
  {file = "map/map8.tmx" , view =  MapView.horizontal, } --  image=Folder.background.."stage8.png", 
}
Triggers =
{
  {type=0,name="dispatch",}
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
  bar="bar",
  fire="fire", 

}
Joystick =
{
  type = 2,
  controlTag = 1,
}
Joypad =
{ 

  { file = "control/jump.png",pos = Vec2(ScreenWidth - 70, 45), name = CommandPad.jump }, 
  { file = "control/fire.png",pos = Vec2(ScreenWidth - 70*2.5, 45), name = "fire", text="",visible=false,},
  { file = "control/bar.png",pos = Vec2(ScreenWidth/2, ScreenHeight-20), name = "bar", text="",visible=true},
}

RoleIndex =
{

}

Role = 
{
  { tag = 1,type = RoleType.player,frame = "player1.png",pos = Vec2(2,12), weapon=gunObject,hp=3,hpMax=3,},

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
      Role[1].ref:getWeapon():shootAt(Vec2.ZERO)
    end
  end
end
preload = function()
  --根据语言载入对话,cpp动态写入语言，必须在函数内执行
  require("dialog".."8-"..Language)
  PopupInfo = require("popup"..Language)
  --预先载入公共事件或属性，比如Role里面的事件
  preloadStage()

  local iter = 1
  Role[iter].fsm = Fsm:new(Role[iter],State8.Player.Routine,nil) iter = iter + 1

  --触发器
  iter = 1 
  Triggers[iter].fsm = Fsm:new(Triggers[iter],nil,nil) iter = iter + 1
end
-- cpp初始化调用，
initiate = function() 
   Ref.stage:loadCurtain({type = CurtainType.bilateral, image={"background/curtainleft.png","background/curtainright.png"},  delay = 1, zorder = ZOrder.dropCurtain})
  State8.initalize()
  Triggers[1].fsm:changeState(State8.Triggers.Dispatch)  
  Resh.playMusic(Music.BubbleBobble2)
  createStar(12,Vec2(8,5),ZOrder.cover)
  globalInit()
end
--循环调用
update = function()
  --每隔5秒
  if(checkRetry()) then
    updateGlobal()
  end 
end