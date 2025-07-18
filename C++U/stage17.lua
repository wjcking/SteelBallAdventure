--测试状态机在里面
require("global")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local NpcStates = require("npcstates")
local CommonStates = require("commonstates")
local Telegram = require("telegram")
local State17 = require("state17")
PopupInfo = nil
Resources =
{  
  {music = Music.Europe},
 
}
Background =
{

}

-- 地图
Atlas =
{
  {file = "map/map17.tmx", view = MapView.panorama} 
}
Triggers =
{

}
--对话范围 cpp下标 0 开始
Segment =
{
  { rangeStart = 0,  rangeEnd= 10, },

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

}
--第一关6-21是混蛋
RoleIndex =
{
  boardStar = 2,
  bastardStart = 3,
  bastardEnd = 25,
  crystal=27,
}
local FrameCrystal   =
{
  {type = FrameStyle.idle,  rangeStart =1, rangeEnd =1,name="crystal",},
  {type = FrameStyle.hurt,  rangeStart =2, rangeEnd =2,},
  {type = FrameStyle.die,  rangeStart =2, rangeEnd =2,},
} 
Role = 
{
  { tag = 1,type = RoleType.player,frame = "player1.png",pos = Vec2(9,41),face = MovingDirection.toTop,insetTile = Vec2(13.0,3.0) },
  { tag = 2,type = RoleType.robject,file = Folder.texture.."adboard_small.png", pos = Vec2(3,25), text=RObjectText.boardStar, offset=Vec2(20,0)},
  --左边
  { tag = 3,type = RoleType.npc,frame = "lady1.png", framePrefix= "lady",pos = Vec2(4,19),face = MovingDirection.toBottom,isHarmful  = false, frameIndexes=FrameTemplate.player, isSolid=true, standPosition=MovingDirection.toRight,},
  { tag = 4,type = RoleType.npc,frame = "kid1.png", framePrefix= "kid",pos = Vec2(5,19),face = MovingDirection.toBottom,isHarmful  = false, frameIndexes=FrameTemplate.player, isSolid=true,standPosition=MovingDirection.toRight,},
  { tag = 5,type = RoleType.npc,frame = "cowboy1.png", framePrefix= "cowboy",pos = Vec2(6,19),face = MovingDirection.toBottom,isHarmful  = false, frameIndexes=FrameTemplate.player,isSolid=true,standPosition=MovingDirection.toRight,},
  { tag = 6,type = RoleType.npc,frame = "kid1.png", framePrefix= "kid",pos = Vec2(3,19),face = MovingDirection.toBottom,isHarmful  = false,  frameIndexes=FrameTemplate.player,isSolid=true,standPosition=MovingDirection.toRight,},
  { tag = 7,type = RoleType.npc,frame = "man1.png", framePrefix= "man",pos = Vec2(7,19),face = MovingDirection.toBottom,isHarmful  = false, frameIndexes=FrameTemplate.player, isSolid=true,standPosition=MovingDirection.toRight,},
  { tag = 8,type = RoleType.npc,frame = "lady1.png", framePrefix= "lady",pos = Vec2(10,19),face = MovingDirection.toBottom,isHarmful  = false, frameIndexes=FrameTemplate.player, isSolid=true,},
  { tag = 9,type = RoleType.npc,frame = "kid1.png", framePrefix= "kid",pos = Vec2(11,19),face = MovingDirection.toBottom,isHarmful  = false, frameIndexes=FrameTemplate.player, isSolid=true,},
  { tag = 10,type = RoleType.npc,frame = "cowboy1.png", framePrefix= "cowboy",pos = Vec2(12,19),face = MovingDirection.toBottom,isHarmful  = false, frameIndexes=FrameTemplate.player, isSolid=true,},
  { tag = 11,type = RoleType.npc,frame = "kid1.png", framePrefix= "kid",pos = Vec2(13,19),face = MovingDirection.toBottom,isHarmful  = false,  frameIndexes=FrameTemplate.player, isSolid=true,},
  { tag = 12,type = RoleType.npc,frame = "man1.png", framePrefix= "man",pos = Vec2(14,19),face = MovingDirection.toBottom,isHarmful  = false, frameIndexes=FrameTemplate.player, isSolid=true,},
  { tag = 13,type = RoleType.npc,frame = "woman1.png", framePrefix= "woman",pos = Vec2(5,20),face = MovingDirection.toBottom,isHarmful  = false,  frameIndexes=FrameTemplate.player,isSolid=true,standPosition=MovingDirection.toRight,},
  { tag = 14,type = RoleType.npc,frame = "lady1.png", framePrefix= "lady",pos = Vec2(6,20),face = MovingDirection.toBottom,isHarmful  = false,  frameIndexes=FrameTemplate.player, isSolid=true,standPosition=MovingDirection.toRight,},

  { tag = 15,type = RoleType.npc,frame = "cowboy1.png", framePrefix= "cowboy",pos = Vec2(7,20),face = MovingDirection.toBottom,isHarmful  = false, frameIndexes=FrameTemplate.player, isSolid=true,standPosition=MovingDirection.toRight,},

  { tag = 16,type = RoleType.npc,frame = "kid1.png", framePrefix= "kid",pos = Vec2(10,20),face = MovingDirection.toBottom,isHarmful = false,  frameIndexes=FrameTemplate.player, isSolid=true,},
  { tag = 17,type = RoleType.npc,frame = "cowboy1.png", framePrefix= "cowboy",pos = Vec2(11,20),face = MovingDirection.toBottom,isHarmful = false,  frameIndexes=FrameTemplate.player, isSolid=true,},
  { tag = 18,type = RoleType.npc,frame = "man1.png", framePrefix= "man",pos = Vec2(12,20),face = MovingDirection.toBottom,isHarmful = false,  frameIndexes=FrameTemplate.player, isSolid=true,},
  { tag = 19,type = RoleType.npc,frame = "woman1.png", framePrefix= "woman",pos = Vec2(13,20),face = MovingDirection.toBottom,isHarmful = false,  frameIndexes=FrameTemplate.player, isSolid=true,},
  { tag = 20,type = RoleType.npc,frame = "woman1.png", framePrefix= "woman",pos = Vec2(6,21),face = MovingDirection.toBottom,isHarmful = false,  frameIndexes=FrameTemplate.player, isSolid=true,standPosition=MovingDirection.toRight,},  
  { tag = 21,type = RoleType.npc,frame = "lady1.png", framePrefix= "lady",pos = Vec2(7,21),face = MovingDirection.toBottom,isHarmful  = false, frameIndexes=FrameTemplate.player, isSolid=true,standPosition=MovingDirection.toRight,},

  { tag = 22,type = RoleType.npc,frame = "kid1.png", framePrefix= "kid",pos = Vec2(10,21),face = MovingDirection.toBottom,isHarmful  = false,  frameIndexes=FrameTemplate.player, isSolid=true,},
  { tag = 23,type = RoleType.npc,frame = "man1.png", framePrefix= "man",pos = Vec2(11,21),face = MovingDirection.toBottom,isHarmful  = false, frameIndexes=FrameTemplate.player, isSolid=true,},
  { tag = 24,type = RoleType.npc,frame = "woman1.png", framePrefix= "woman",pos = Vec2(12,21),face = MovingDirection.toBottom,isHarmful  = false,  frameIndexes=FrameTemplate.player, isSolid=true,},
  { tag = 25,type = RoleType.npc,frame = "lady1.png", framePrefix= "lady",pos = Vec2(13,21),face = MovingDirection.toBottom,isHarmful  = false,  frameIndexes=FrameTemplate.player, isSolid=true,},
  --广告牌 水晶
  { tag = 26,type = RoleType.robject, file=Folder.texture.."adboard_small.png", framePrefix="",pos = Vec2(2,16),  isSolid=true,offset=Vec2(29,-10),text=RObjectText.boardCrystal,},
  { tag = 27,type = RoleType.robject,frame = "crystal.png", framePrefix="crystal",pos = Vec2(9,17), animate = Animate.hoverCrystal, isSolid=false, frameIndexes=FrameCrystal,offset=Vec2(-20, 5)},
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
  require("dialog".."17-"..Language)
  PopupInfo = require("popup"..Language)
  --预先载入公共事件或属性，比如Role里面的事件
  preloadStage()

  local iter = 1

  Role[iter].fsm = Fsm:new(Role[iter],State17.Player.Routine) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1

  repeat
    Role[iter].onCheckTile=false
    Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  until(iter > RoleIndex.bastardEnd)
end
-- cpp初始化调用，
initiate = function()
  Ref.stage:loadCurtain({type = CurtainType.updown, image="background/curtaintop.png",text="", delay=0.5 })
  Resh.playMusic(Music.Europe)
  State17.cheers()
  local starPositions = 
  {
    Vec2(6,27),
    Vec2(5,28),
    Vec2(4,29),
    Vec2(5,30),Vec2(6,31),Vec2(5,29),
    Vec2(11,27),Vec2(12,28),Vec2(13,29),Vec2(12,30),Vec2(11,31),Vec2(12,29),
   }
   --星星祭坛 
   local starCount = getStarCount()
   if (starCount > #starPositions) then
     starCount = #starPositions
     Role[RoleIndex.boardStar].ref:registerText(RObjectText.boardStarDone)
   Ref.stage:promptAchieve(AchieveIndex.Star12,PopupInfo.achieve(AchieveIndex.Star12))
  end
  if starCount > 4 then
     local flagRed = { type = RoleType.robject,onCheckTile=false, file=Folder.texture.."eu_red.png",pos=Vec2(6,37), isSolid=true, face=MovingDirection.toLeft, offset=Vec2(15,0), }
     flagRed.fsm = Fsm:new(flagRed,nil,nil)
     createRole(flagRed)
  end
    if starCount > 11 then
     local flagGreen = { type = RoleType.robject,onCheckTile=false, file=Folder.texture.."eu_green.png",pos=Vec2(11,37), isSolid=true,   offset=Vec2(-15,0), }
     flagGreen.fsm = Fsm:new(flagGreen,nil,nil)
     createRole(flagGreen)
  end
   for i=1,starCount,1 do
    local star = { type = RoleType.robject,onCheckTile=false,zorder=1,pos=starPositions[i],frame="star.png",animate= {type = Anistyle.rebound,limitedTimes=-1, offset=Vec2(0,2), duration=1, durationBack=1, delay=0,delayBack=0},}
     star.fsm = Fsm:new(star,nil,nil)
     createRole(star)
   end
   globalInit()
end
--循环调用
update = function()
  updateGlobal()
end