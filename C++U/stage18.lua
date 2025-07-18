--测试状态机在里面
require("global")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local NpcStates = require("npcstates")
local Telegram = require("telegram")
local CommonStates = require("commonstates")
local State18= require("state18")
PopupInfo = nil
Resources =
{  
  { music = Music.LittleApple }, 
  {plist = Folder.texture.."auto"},--不要加扩展名.png
}
Background =
{
  {file= Folder.background.."dusk.png",},
}
-- 地图 
Atlas =
{
  {file = "map/map18.tmx" , view =  MapView.horizontal,pos=Vec2(18,0), }  
}
Triggers =
{
  {type=0,name="",},
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
  jump="jump",
}
Joystick =
{
  type = 2,
  controlTag = 1, 

}
Joypad =
{
  -- { file = "control/jump.png",pos = Vec2(ScreenWidth - 70, 45), name = CommandPad.jump },

}

RoleIndex =
{
  sun = 2,
  credit=8,
  roadsign =9,
  flag=10,
  lady=11,
  women=12,
  kid=13,
  man=14,
  cowboy=15,
}
local FrameHappy=
{
  {type = FrameStyle.idle,  rangeStart =3, rangeEnd =4, name="player",},
  {type = FrameStyle.faceUP,  rangeStart =6, rangeEnd =6,},
  {type = FrameStyle.faceDown,  rangeStart =10, rangeEnd =10,},
  {type = FrameStyle.walkX,  rangeStart =3, rangeEnd =4,},
  {type = FrameStyle.walkUP,  rangeStart =6, rangeEnd =8,},
  {type = FrameStyle.walkDown,  rangeStart =10, rangeEnd =12,},
  {type = FrameStyle.fire,  rangeStart =5, rangeEnd =5,},
  {type = FrameStyle.fireUP,  rangeStart =9, rangeEnd =9,},
  {type = FrameStyle.fireDown,  rangeStart =13, rangeEnd =13,},
  {type = FrameStyle.jump,  rangeStart =3, rangeEnd =4,},
  {type = FrameStyle.hurt,  rangeStart =14, rangeEnd =14,},
  {type = FrameStyle.die,  rangeStart =15, rangeEnd =19,},
}
local  rolePos = Vec2(35,9)
local roleOffset = Vec2(16,0)
local roleStep=Vec2(0.11,0.11)
Role = 
{
  { tag = 1,type = RoleType.player,frame = "player1.png",pos = rolePos,face=MovingDirection.toLeft, frameIndexes=FrameHappy, step=roleStep, },
  { tag = 2,type = RoleType.robject,frame = "sun1.png", frameIndexes=FrameTemplate.sun,pos = Vec2(21,4), offset=Vec2(0,0),  allowGravity=false,zorder=ZOrder.cover,onCheckTile=true, isSolid=false,sound=Sound.sting, allowFlip=false, step=roleStep,animate=Animate.hoverSun},
  { tag = 3,type = RoleType.robject,frame = "cloud11.png",pos = Vec2(16,4), offset=Vec2(0,0),  direction=MovingDirection.toRight,  allowGravity=false,zorder=ZOrder.cover,onCheckTile=true, isSolid=false,isHarmful=false,step=stepCloud, distance=Vec2(ScreenWidth*1.4,ScreenHeight), allowFlip=false, isExcepted=true, step=Vec2(0.2,0.2), allowTouch=true, touchTimes=0, isDropped = false},
  { tag = 4,type = RoleType.robject,frame = "cloud21.png",pos = Vec2(12,5), offset=Vec2(0,0),  direction=MovingDirection.toRight,   allowGravity=false,zorder=ZOrder.cover,onCheckTile=true, isSolid=false,isHarmful=false,step=stepCloud,distance=Vec2(ScreenWidth*1.6,ScreenHeight), allowFlip=false, zorder=ZOrder.overlay,isExcepted=true, step=Vec2(0.2,0.2),allowTouch=true,touchTimes=0, isDropped = false},
  { tag = 5,type = RoleType.robject,frame = "cloud31.png",pos = Vec2(8,4), offset=Vec2(0,0),  direction=MovingDirection.toRight,  allowGravity=false,zorder=ZOrder.cover,onCheckTile=true, isSolid=false,isHarmful=false,step=stepCloud,distance=Vec2(ScreenWidth*1.8,ScreenHeight), allowFlip=false,isExcepted=true, step=Vec2(0.2,0.2),allowTouch=true,touchTimes=0, isDropped = false},
  { tag = 6,type = RoleType.robject,frame = "cloud11.png",pos = Vec2(4,5), offset=Vec2(0,0),  direction=MovingDirection.toRight,  allowGravity=false,zorder=ZOrder.cover,onCheckTile=true, isSolid=false,isHarmful=false,step=stepCloud,distance=Vec2(ScreenWidth*2,ScreenHeight), allowFlip=false,zorder=ZOrder.overlay,isExcepted=true, step=Vec2(0.2,0.2),allowTouch=true,touchTimes=0, isDropped = false},
  { tag = 7,type = RoleType.robject,frame = "cloud21.png",pos = Vec2(0,4), offset=Vec2(0,0),  direction=MovingDirection.toRight,  allowGravity=false,zorder=ZOrder.cover,onCheckTile=true, isSolid=false,isHarmful=false,step=stepCloud,distance=Vec2(ScreenWidth*2.2,ScreenHeight), allowFlip=false,isExcepted=true, step=Vec2(0.2,0.2),allowTouch=true,touchTimes=0, isDropped = false},  
  { tag = 8,type = RoleType.robject,file = Folder.texture.."dialog_cloud.png", pos = Vec2(16,5), allowFlip=false, isSolid = false,onCheckTile=false,allowGravity=false,direction=MovingDirection.toRight,distance=Vec2(ScreenWidth*1.2,ScreenHeight),isExcepted=true, step=Vec2(0.5,0.5),text=Credit[1],zorder=10,allowTouch=true,touchTimes=0, isDropped = false},
  --roadsign
  { tag = 9,type = RoleType.robject,frame = "roadsign.png",pos = Vec2(16,8), offset=Vec2(0,-18),  allowGravity=false,zorder=ZOrder.cover,onCheckTile=false, isSolid=false,  allowFlip=false, direction  = MovingDirection.stayStill,step=Vec2(3,3),isExcepted=true, },
  { tag = 10,type = RoleType.robject, file=Folder.texture.."eu_blue.png",  pos = Vec2(rolePos.x + 1,rolePos.y-1),allowGravity=false,onCheckTile=false, allowFilp=false, face=MovingDirection.toRight, offset=Vec2(0,0),step=roleStep, visible=false,zorder=1000,},
  --跟随
  { tag = 11,type = RoleType.player,frame = "cowboy1.png", framePrefix="cowboy",pos = Vec2(rolePos.x + 3,rolePos.y),face=MovingDirection.toLeft, frameIndexes=FrameHappy, offset=roleOffset,step=roleStep,},
  { tag = 12,type = RoleType.player,frame = "woman1.png", framePrefix="woman",pos = Vec2(rolePos.x + 4,rolePos.y),face=MovingDirection.toLeft, frameIndexes=FrameHappy, offset=roleOffset,step=roleStep,},
  { tag = 13,type = RoleType.player,frame = "kid1.png", framePrefix="kid",pos = Vec2(rolePos.x + 5,rolePos.y),face=MovingDirection.toLeft, frameIndexes=FrameHappy, offset=roleOffset,step=roleStep,},
  { tag = 14,type = RoleType.player,frame = "man1.png", framePrefix="man",pos = Vec2(rolePos.x + 6,rolePos.y),face=MovingDirection.toLeft, frameIndexes=FrameHappy, offset=roleOffset,step=roleStep,},
  { tag = 15,type = RoleType.player,frame = "lady1.png", framePrefix="lady",pos = Vec2(rolePos.x + 7,rolePos.y),face=MovingDirection.toLeft, frameIndexes=FrameHappy, offset=roleOffset,step=roleStep,},
  { tag = 16,type = RoleType.player,frame = "wb2.png", framePrefix="",pos = Vec2(rolePos.x + 2,rolePos.y),offset=Vec2(0,0),face=MovingDirection.toLeft,   offset=roleOffset,step=roleStep,},
  { tag = 17,type = RoleType.player,frame = "bird_gray1.png", framePrefix="bird_gray",pos = Vec2(rolePos.x + 4,rolePos.y-3),offset=Vec2(35,0),face=MovingDirection.toLeft, allowGravity=false, frameIndexes=FrameTemplate.bird , offset=roleOffset,step=roleStep,},
      { tag = 18,type = RoleType.player,frame = "bird_gray1.png", framePrefix="bird_gray",pos = Vec2(rolePos.x + 5,rolePos.y-5),offset=Vec2(35,0),face=MovingDirection.toLeft, allowGravity=false, frameIndexes=FrameTemplate.bird, offset=roleOffset,step=roleStep,},
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
  if  className == "stage" and  Ref.stage:isTypeDone() then 

    Resh.playMusic(Music.LittleApple) 
    Role[1].fsm:changeState(State18.Npc.Show)
    Role[RoleIndex.credit].fsm:changeState(State18.RObject.Respawn)

    Role[RoleIndex.lady].fsm:changeState(State18.Npc.Show)
    Role[RoleIndex.women].fsm:changeState(State18.Npc.Show)
    Role[RoleIndex.kid].fsm:changeState(State18.Npc.Show)
    Role[RoleIndex.man].fsm:changeState(State18.Npc.Show)
    Role[RoleIndex.cowboy].fsm:changeState(State18.Npc.Show)
    
    Role[16].fsm:changeState(State18.Npc.Show) 
    Role[17].fsm:changeState(State18.Npc.Show) 
    Role[18].fsm:changeState(State18.Npc.Show)

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
  require("dialog".."18-"..Language)
  PopupInfo = require("popup"..Language)
  --预先载入公共事件或属性，比如Role里面的事件
  preloadStage()

  local iter = 1
  Role[iter].fsm = Fsm:new(Role[iter],PlayerStates.Routine, nil ) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1

  Role[iter].fsm = Fsm:new(Role[iter],nil, State18.RObject.Respawn) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, State18.RObject.Respawn) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, State18.RObject.Respawn) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, State18.RObject.Respawn) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, State18.RObject.Respawn) iter = iter + 1
  --credit
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  --roadsign
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1

  Role[iter].fsm = Fsm:new(Role[iter],nil, nil ) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  
  
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil, nil) iter = iter + 1

  --触发器
  iter = 1 

end
-- cpp初始化调用，
initiate = function() 
  State18.initialize()

 -- Ads.showBanner(Banner)
  Ref.stage:loadCurtain({type = CurtainType.black,   delay = 4, zorder = ZOrder.robject})

  Joystick.ref:hideJoystick()
  getStage():setPtc(Vec2(18,11),Vec2.ZERO)
  --欧盟旗帜
  if (getStarCount() >= 12) then
    Role[RoleIndex.flag].ref.isVisible = true
    Role[1].ref:setCarriedOffset(Vec2(-10,40))
    Role[1].ref:carryObject(Role[RoleIndex.flag].ref)
  end
  globalInit()
end
--循环调用
update = function()
  updateGlobal() 
end