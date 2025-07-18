--测试状态机在里面
require("global")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local NpcStates = require("npcstates")
local CommonStates = require("commonstates")
local Telegram = require("telegram")
local State1 = require("state1")
PopupInfo = nil
Resources =
{  
  {music = Music.overWorld},
  {music = Music.dungeonBoss},
  {sound = Sound.wangbaHurt },
}
Background =
{
  {file= Folder.background.."sky.png",},
}

-- 地图
Atlas =
{
  {file = "map/map1.tmx", view = MapView.panorama} 
}
Triggers =
{
  {type=TriggerType.rect, name="start", rect=Rect(250, 520, 350, 220), isActive=true}, 
  {type=TriggerType.rect, name="block", rect=Rect(100, 340, 80, 40), isActive=true}, 
}
--对话范围 cpp下标 0 开始
Segment =
{
  { rangeStart = 0,  rangeEnd= 10, },
  { rangeStart = 11,  rangeEnd= 22, },
  { rangeStart = 23,  rangeEnd= 28, },
  { rangeStart = 29,  rangeEnd= 34, },
}
--如果lua中上一次的collapse存在，务必重置后才能使用否则会出现range越界,定义变量不赋值则可以清空
Collapse =
{

}

Joystick = { type = 1, controlTag = 1, }
CommandPad = 
{
  barSteelball="barSteelball",
  barDick ="barDick",
  attack1 = "attack1", attack2="attack2",dodge="dodge",escape="escape",hand="hand",
}

Joypad =
{
  { file = "control/bar.png",pos = Vec2(ScreenWidth/3, ScreenHeight-20), name = "barSteelball", text="",visible=false},
  { file = "control/bar.png",pos = Vec2(ScreenWidth/1.5, ScreenHeight-20), name = "barDick", text="",visible=false},
  { file = "control/button.png",pos = Vec2(ScreenWidth - 85, 40), name = "attack1", text="",visible=false },
  { file = "control/button.png",pos = Vec2(ScreenWidth - 85 * 2, 40), name = "attack2", text="",visible=false},
  { file = "control/button.png",pos = Vec2(ScreenWidth - 85 * 3, 40), name = "hand", text="",visible=false},
  { file = "control/button.png",pos = Vec2(ScreenWidth - 85 * 4, 40), name = "dodge", text="",visible=false},
  { file = "control/button.png",pos = Vec2(ScreenWidth - 85 * 5, 40), name = "escape", text="",visible=false},
}
--第一关6-21是混蛋
RoleIndex =
{
  wangba =22,
  bat =23,
  board = 5,
  adboard = 24,
  lady6 = 6,
  woman19=19,
  bastardStart=6,
  bastardEnd=20,
  crystal = 21,
}

local FrameButtonRed =  {name="button_red", rangeStart=1,rangeEnd=2,direction=-3,sound = Sound.button,limitedTimes=-1,  }
local FrameButtonGreen =  {name="button_green", rangeStart=1,rangeEnd=2,direction=-3,sound =Sound.button,limitedTimes=1, }
local FrameButtonBlue =  {name="button_blue", rangeStart=1,rangeEnd=2,direction=-3,sound = Sound.button,limitedTimes=-1, }
 local FrameCrystal   =
  {
    {type = FrameStyle.idle,  rangeStart =1, rangeEnd =1,name="crystal",},
    {type = FrameStyle.hurt,  rangeStart =2, rangeEnd =2,},
    {type = FrameStyle.die,  rangeStart =2, rangeEnd =2,},
  } 
 local  jumpLaugh= { type = Anistyle.jump, height=5,duration=0.25}
Role = 
{
  { tag = 1,type = RoleType.player,frame = "player1.png",pos = Vec2(14,22),face = MovingDirection.toTop,insetTile = Vec2(13.0,3.0) },
  { tag = 2,type = RoleType.robject,frame = "button_red1.png", framePrefix="button_red", buttonIndex = 0,pos = Vec2(7,21), isSolid=false, switch=FrameButtonRed},
  { tag = 3,type = RoleType.robject,frame = "button_green1.png",framePrefix="button_green",buttonIndex = 1,pos = Vec2(8,19),isSolid=false,  switch=FrameButtonGreen},
  { tag = 4,type = RoleType.robject,frame = "button_blue1.png",framePrefix="button_blue",buttonIndex = 2,pos = Vec2(9,21),isSolid=false,  switch=FrameButtonBlue},
  { tag = 5,type = RoleType.robject,file = Folder.texture.."adboard_small.png", pos = Vec2(8,19), allowPush = CollisionDirection.atBottom},
  --bastard
  { tag = 6,type = RoleType.npc,frame = "lady1.png", framePrefix= "lady",pos = Vec2(5,10),face = MovingDirection.toTop,isHarmful  = false, frameIndexes=FrameTemplate.player,   },
  { tag = 7,type = RoleType.npc,frame = "kid1.png", framePrefix= "kid",pos = Vec2(6,10),face = MovingDirection.toTop,isHarmful  = false, frameIndexes=FrameTemplate.player, },
  { tag = 8,type = RoleType.npc,frame = "cowboy1.png", framePrefix= "cowboy",pos = Vec2(7,11),face = MovingDirection.toTop,isHarmful  = false, frameIndexes=FrameTemplate.player,},
  { tag = 9,type = RoleType.npc,frame = "kid1.png", framePrefix= "kid",pos = Vec2(8,10),face = MovingDirection.toTop,isHarmful  = false,  frameIndexes=FrameTemplate.player, animate={ type = Anistyle.jump, height=12,duration=0.35},},
  { tag = 10,type = RoleType.npc,frame = "man1.png", framePrefix= "man",pos = Vec2(9,11),face = MovingDirection.toTop,isHarmful  = false, frameIndexes=FrameTemplate.player, },
  { tag = 11,type = RoleType.npc,frame = "woman1.png", framePrefix= "woman",pos = Vec2(10,10),face = MovingDirection.toTop,isHarmful  = false,  frameIndexes=FrameTemplate.player, animate={ type = Anistyle.jump, height=8,duration=0.35},},
  { tag = 12,type = RoleType.npc,frame = "lady1.png", framePrefix= "lady",pos = Vec2(11,11),face = MovingDirection.toTop,isHarmful  = false,  frameIndexes=FrameTemplate.player,},
  { tag = 13,type = RoleType.npc,frame = "cowboy1.png", framePrefix= "cowboy",pos = Vec2(12,10),face = MovingDirection.toTop,isHarmful  = false, frameIndexes=FrameTemplate.player, animate=jumpLaugh,},
  { tag = 14,type = RoleType.npc,frame = "cowboy1.png", framePrefix= "cowboy",pos = Vec2(4,12),face = MovingDirection.toTop,isHarmful  = false,  frameIndexes=FrameTemplate.player,},
  { tag = 15,type = RoleType.npc,frame = "woman1.png", framePrefix= "woman",pos = Vec2(9,13),face = MovingDirection.toTop,isHarmful = false,  frameIndexes=FrameTemplate.player,animate={ type = Anistyle.jump, height=10,duration=0.25},},
  { tag = 16,type = RoleType.npc,frame = "kid1.png", framePrefix= "kid",pos = Vec2(8,12),face = MovingDirection.toTop,isHarmful = false,  frameIndexes=FrameTemplate.player,},
  { tag = 17,type = RoleType.npc,frame = "cowboy1.png", framePrefix= "cowboy",pos = Vec2(8,13),face = MovingDirection.toTop,isHarmful = false,  frameIndexes=FrameTemplate.player, },
  { tag = 18,type = RoleType.npc,frame = "man1.png", framePrefix= "man",pos = Vec2(4,13),face = MovingDirection.toTop,isHarmful = false,  frameIndexes=FrameTemplate.player,},
  { tag = 19,type = RoleType.npc,frame = "woman1.png", framePrefix= "woman",pos = Vec2(13,13),face = MovingDirection.toTop,isHarmful = false,  frameIndexes=FrameTemplate.player,},
  { tag = 20,type = RoleType.npc,frame = "woman1.png", framePrefix= "woman",pos = Vec2(6,13),face = MovingDirection.toTop,isHarmful = false,  frameIndexes=FrameTemplate.player,animate={ type = Anistyle.jump, height=9,duration=0.4},},
  { tag = 21,type = RoleType.robject,frame = "crystal.png", framePrefix="crystal",pos = Vec2(9,8), animate = Animate.hoverCrystal, isSolid=false, frameIndexes=FrameCrystal,},
  { tag = 22,type = RoleType.npc,frame = "wangba1.png", framePrefix="wangba",pos = Vec2(9,2), step=Vec2(0.5,0.5) , isSolid=false, frameIndexes=FrameTemplate.wangba,onCheckTile=false,},
  { tag = 23,type = RoleType.robject, file=Folder.texture.."bat.png", framePrefix="",pos = Vec2(4,7),  isSolid=false,offset=Vec2(0,-10) },
  { tag = 24,type = RoleType.robject, file=Folder.texture.."adboard_small.png", framePrefix="",pos = Vec2(3,7),  isSolid=true,offset=Vec2(29,-10),allowTouch=true},
}

local function clapWangba()
  --如果棒子没有拾起来
  if (Role[RoleIndex.bat].ref.isVisible or Role[RoleIndex.wangba].ref:isDead()) then
    return
  end
  local rect = Role[RoleIndex.wangba].ref:getCollisionBound(Role[RoleIndex.wangba].ref:getInsetObject(),false)
  local clap =  Ref.stage:isSlided(Vec2(0,10),rect)
  --棒子捡起来后直接隐藏
  State1.setPadVisible(false)
  if (clap == 0) then
    return
  end
  
  Resh.playSound(Sound.hitWangba)
  if (clap == SlidedResult.left) then 
    Role[RoleIndex.wangba].ref:gotHurt(250,"")
    Role[RoleIndex.wangba].ref:setFrameIndex(FrameStyle.idle,5,5)
    Role[RoleIndex.wangba].ref:setFrameIndex(FrameStyle.hurt,5,5)
    Role[22].ref.isFlippedX = false
  elseif(clap== SlidedResult.right) then 
    Role[RoleIndex.wangba].ref:gotHurt(250,"")
    Role[RoleIndex.wangba].ref:setFrameIndex(FrameStyle.idle,5,5)
    Role[RoleIndex.wangba].ref:setFrameIndex(FrameStyle.hurt,5,5)
    Role[RoleIndex.wangba].ref.isFlippedX = true
  end
  Role[RoleIndex.wangba].ref:stopAnimation()
  Role[RoleIndex.wangba].ref:setAnimation(Animate.getToastDelay("HP-250",0.5))
  Joystick.ref:setString(CommandPad.barDick,RObjectText.barDick.text..tostring(Role[22].ref.hp))
end

local function retry() 
  Joystick.ref:setAllVisible(true)
  Delay.reset(DelayIndex.recover)
  Role[1].ref:closeDialog()
  Role[1].isRecovered = nil
  Role[1].ref:recover()
  Role[1].ref:stop()
  Role[1].ref.face = MovingDirection.toTop
  Role[1].ref:spawn(Vec2(5,11),Vec2.ZERO)
  Role[1].ref.hp=1000
  Role[RoleIndex.wangba].ref.hp=2000
  Role[RoleIndex.wangba].ref:recover()
  Role[RoleIndex.wangba].ref:closeDialog()
  Role[RoleIndex.adboard].ref:spawn(Vec2(3,7),Vec2(29,-10))
 -- Role[RoleIndex.adboard].ref:setAllowDragY(false)
  Role[RoleIndex.adboard].ref.isVisible = true
  Role[RoleIndex.adboard].ref:registerText(RObjectText.adDick)
  Role[RoleIndex.bat].ref.isVisible = true
  Role[RoleIndex.wangba].ref:setFrame(FrameStyle.idle,1,1)
  Role[RoleIndex.wangba].ref:setFrame(FrameStyle.hurt,3,3)
  Joystick.ref:setString(CommandPad.barSteelball,RObjectText.barSteelball.text..tostring(Role[1].ref.hp))
  Joystick.ref:setString(CommandPad.barDick,RObjectText.barDick.text..tostring(Role[22].ref.hp))
  --去除所有子弹
  for _,each in ipairs(Role) do
    if (each.type == RoleType.projectTile) then
      each.isDisposed = true
    end
  end
end
--回调事件，funcName函数名
callback = function(className,funcName,params) 
  --全局回调函数
  globalCallFunc(className,funcName,params)

  if (className == "gun" and funcName =="shoot") then
    RoleModel.addProjectObject(params.ref)
  end
  
--对话放在冻结前面
  if (className == "stage") then
    --撸王八
    clapWangba()
    --对话
    DialogueSystem.proceed()

    if (funcName == "ended") then
      if (not Role[RoleIndex.adboard].ref.isVisible and
        Role[RoleIndex.bat].ref.isVisible and
        Role[RoleIndex.bat].ref:isTouched(Ref.stage:getEndDelta())) then
        Role[RoleIndex.bat].ref.isVisible = false
        Ref.stage:prompt(PopupInfo.createObject(Folder.texture.."bigbat.png",RObjectText.bat))
      State1.setPadVisible(false)
      end
    end
    if (funcName == "dialog" and params.name == "recover") then
      retry()
    end
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
    -- Role[22].ref:getWeapon():shootAt(Vec2.ZERO)
    if (Joystick.ref:getTouchedPad(1)) then  
      Role[1].ref:getWeapon():shootAt(Vec2(0,0))
    end
  end
end

preload = function()
  --根据语言载入对话,cpp动态写入语言，必须在函数内执行
  require("dialog".."1-"..Language)
  PopupInfo = require("popup"..Language)
  PopupInfo = require("popup"..Language)
  --预先载入公共事件或属性，比如Role里面的事件
  preloadStage()
  Triggers[1].fsm = Fsm:new(Triggers[1],CommonStates.Triggers.ScanOnce,nil)
  Triggers[2].fsm = Fsm:new(Triggers[2],State1.Trigger.Block,nil)

  Role[1].fsm = Fsm:new(1,State1.Player.Routine)
  Role[2].fsm = Fsm:new(2,nil,State1.RObject.Button)
  Role[3].fsm = Fsm:new(3,nil,State1.RObject.Button)
  Role[4].fsm = Fsm:new(4,nil,State1.RObject.Button)
  Role[5].fsm = Fsm:new(5,nil,nil)
  --bastard
  Role[6].fsm =  Fsm:new(6,State1.Npc.Routine,State1.Npc.BastardAndCrystal)
  Role[7].fsm =  Fsm:new(7,State1.Npc.Routine,State1.Npc.BastardAndCrystal)
  Role[8].fsm =  Fsm:new(8,State1.Npc.Routine,State1.Npc.BastardAndCrystal)
  Role[9].fsm =  Fsm:new(9,State1.Npc.Routine,State1.Npc.BastardAndCrystal)
  Role[10].fsm = Fsm:new(10,State1.Npc.Routine,State1.Npc.BastardAndCrystal)
  Role[11].fsm = Fsm:new(11,State1.Npc.Routine,State1.Npc.BastardAndCrystal)
  Role[12].fsm = Fsm:new(12,State1.Npc.Routine,State1.Npc.BastardAndCrystal)
  Role[13].fsm = Fsm:new(13,State1.Npc.Routine,State1.Npc.BastardAndCrystal)
  Role[14].fsm = Fsm:new(14,State1.Npc.Routine,State1.Npc.BastardAndCrystal)
  Role[15].fsm = Fsm:new(15,State1.Npc.Routine,State1.Npc.BastardAndCrystal)
  Role[16].fsm = Fsm:new(16,State1.Npc.Routine,State1.Npc.BastardAndCrystal)
  Role[17].fsm = Fsm:new(17,State1.Npc.Routine,State1.Npc.BastardAndCrystal)
  Role[18].fsm = Fsm:new(18,State1.Npc.Routine,State1.Npc.BastardAndCrystal)
  Role[19].fsm = Fsm:new(19,State1.Npc.Routine,State1.Npc.BastardAndCrystal)
  Role[20].fsm = Fsm:new(20,State1.Npc.Routine,State1.Npc.BastardAndCrystal)
--end
  Role[21].fsm = Fsm:new(21,State1.Npc.Routine,nil)
  Role[22].fsm = Fsm:new(22,State1.Wangba.Routine,nil)
  Role[23].fsm = Fsm:new(23,nil,nil)
  Role[24].fsm = Fsm:new(24,nil,nil)
end
-- cpp初始化调用，
initiate = function()

  Ref.stage:loadCurtain({type = CurtainType.black,   delay = 4, zorder = ZOrder.robject})
  Resh.playMusic(Music.overWorld) 
  Ref.stage:output("stage1 language:"..Language)
  local   gunPlayer = {count=-1,  sound=Folder.sound.."toygun.wav",rate=1.5, frame="bullet1.png",project={type=2,step=Vec2(3,3),target=22} }
  local   gunWangba = {count=-1,  sound=Folder.sound.."poogun.wav",rate=0.3, frame="poo.png",project={type=2,step=Vec2(3,3),target=1,offset=Vec2(0,25)}}

  Role[1].ref:getWeapon():registerWeapon(gunPlayer)
  Role[22].ref:getWeapon():registerWeapon(gunWangba)
  Role[5].ref:registerText(RObjectText.notice)

  Role[5].fsm:changeState(RObjectStates.gotPushed)
  Role[24].ref:registerText(RObjectText.ad)
  
  globalInit()
end
--循环调用
update = function()
 updateGlobal()
end