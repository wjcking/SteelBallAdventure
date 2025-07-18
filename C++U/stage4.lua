--测试状态机在里面
require("global")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local NpcStates = require("npcstates")
local Telegram = require("telegram")
local CommonStates = require("commonstates")
local State4= require("state4")
PopupInfo = nil
Resources =
{  
  {music = Music.Rhapsody},
  {sound = Sound.turnPage}, 
  {plist = Folder.texture.."auto"},--不要加扩展名.png
}
Background =
{
  {file= Folder.background.."sky.png",},
}
-- 地图
Atlas =
{
  {file = "map/map4.tmx" , view =  MapView.horizontal,  image=Folder.background.."building.png" } -- 
}
Triggers =
{
  {tag=nil,type=0, name="drip",  isActive=true}, 
  {tag=nil,type=0, name="spawnCar1",  isActive=true},--change多次，每次只能执行一个spawn，否则覆盖
  --几何碰撞,必须加tag 记录每一个上次碰撞位置y
  {tag = 3,type=TriggerType.line, name="line1", rangeStart=Vec2(390,296),rangeEnd=Vec2(666,296), ac=0}, 
  --广告栏
  {tag = 4,type=TriggerType.line, name="line2", rangeStart=Vec2(500+35+390,300),rangeEnd=Vec2(500 + 230 + 666 ,300), ac=0}, 
  --房顶
  {tag= 5,type=TriggerType.line, name="line3", rangeStart=Vec2(2030,292),rangeEnd=Vec2(2266,292), ac=0},

  {tag= 6,type=0, name="spawnCar2",  isActive=true}, 
--房顶
  {tag= 7,type=TriggerType.line, name="line4", rangeStart=Vec2(2400,294),rangeEnd=Vec2(2644,294), ac=0},
}

--对话范围 cpp下标 0 开始
Segment =
{
  { rangeStart = 0,  rangeEnd= 4, },
}


--如果lua中上一次的collapse存在，务必重置后才能使用否则会出现range越界,定义变量不赋值则可以清空
Collapse =
{

}
CommandPad = 
{
  jump = "jump",
  drop = "drop",
  open = "open",
}
Joystick =
{
  type = 2,
  controlTag = 1,
}
Joypad =
{
  { file = "control/jump.png",pos = Vec2(ScreenWidth - 70, 45), name = CommandPad.jump },
  { file = "control/open.png",pos = Vec2(ScreenWidth - 70 * 2.5, 45), name = CommandPad.open, visible= false, },
  { file = "control/drop.png",pos = Vec2(ScreenWidth - 70 * 4, 45), name = CommandPad.drop, visible=false, },
}

RoleIndex =
{
  --triggers
  drip = 1,
  spawnCar1 = 2,
  spawnCar2 = 5,
  giftLine = 7,
  --role
  sign=2,
  cloud=3,
  kid = 4,
  man = 5,
  woman = 6,
  lady = 7,
  cowboy = 8,
  ad1= 10,
  ad2=11,
  ad3=12,
  car2=13,
  questionRed=14,
  block1= 16,
  block2= 17,
  block3= 18,
  block4= 19,
  gift=20,
  tlight=15,
  zzi=21,
  wangba =22,
  map=23,
}
--4个砖块
local knockChar =
{ 
  {type = KnockAction.fetchChar,direction=CollisionDirection.atBottom, sound=Sound.hitChar},
}
local language =Resh.getLanguage()
Role = 
{
  { tag = 1,type = RoleType.player,frame = "player1.png",pos = Vec2(4,17) },
  { tag = 2,type = RoleType.robject,file = Folder.texture.."sign1_"..language..".png",pos = Vec2(7,16), offset=Vec2(0, -18), isSolid=false,allowGravity=false,onCheckTile=false, step=Vec2(4,4),sound=Sound.sting,},
  { tag = 3,type = RoleType.robject,frame =  "cloud11.png",pos = Vec2(9,12), isSolid=true,allowGravity=false,onCheckTile=false,insetY=10, ac=CollisionDirection.atTop,zorder=ZOrder.role + 1, animate=Animate.jumpShake,offset=Vec2(0,20),step=Vec2(0.3,0.3), sound=Sound.sting,},
  { tag = 4,type = RoleType.npc,frame = "kid1.png", frameIndexes=FrameTemplate.player, framePrefix="kid", pos = Vec2(21,17), isHarmful=false,face=MovingDirection.toTop, step=Vec2(0.05,0.05)},
  { tag = 5,type = RoleType.npc,frame = "man1.png", frameIndexes=FrameTemplate.player,framePrefix="man",pos = Vec2(22,17), isHarmful=false,face=MovingDirection.toTop , step=Vec2(0.05,0.05)},
  { tag = 6,type = RoleType.npc,frame = "woman1.png", frameIndexes=FrameTemplate.player,framePrefix="woman",pos = Vec2(23,17), isHarmful=false,face=MovingDirection.toTop , step=Vec2(0.05,0.05)},
  { tag = 7,type = RoleType.npc,frame = "lady1.png", frameIndexes=FrameTemplate.player,framePrefix="lady",pos = Vec2(25,17), isHarmful=false,face=MovingDirection.toTop, step=Vec2(0.05,0.05)},
  { tag = 8,type = RoleType.npc,frame = "cowboy1.png", frameIndexes=FrameTemplate.player,framePrefix="cowboy",pos = Vec2(26,17), isHarmful=false,face=MovingDirection.toTop , step=Vec2(0.05,0.05)},
  { tag = 9,type = RoleType.robject,file = Folder.texture.."adks_"..language..".png",pos = Vec2(21,15), offset=Vec2(-25, -10), isSolid=false,allowGravity=false,onCheckTile=false, zorder=0, allowTouch=true,},
  { tag = 10,type = RoleType.robject,file = Folder.texture.."ad1_"..language..".png",pos = Vec2(23,15), offset=Vec2(-15, -10), isSolid=false,allowGravity=false,onCheckTile=false, zorder=0, allowTouch=true,},
  { tag = 11,type = RoleType.robject,file = Folder.texture.."ad2_"..language..".png",pos = Vec2(25,15), offset=Vec2(0, -10), isSolid=false,allowGravity=false,onCheckTile=false, zorder=0, allowTouch=true,},
  { tag = 12,type = RoleType.robject,file = Folder.texture.."ad3_"..language..".png",pos = Vec2(27,15), offset=Vec2(15, -10), isSolid=false,allowGravity=false,onCheckTile=false, zorder=0, allowTouch=true,},
  { tag = 13,type = RoleType.robject,frame="jeep1.png",frameIndexes=FrameTemplate.jeep, framePrefix="jeep",pos = Vec2(44,17), offset=Vec2(0, 8), isSolid=false, visible=false, face=MovingDirection.toLeft,frameSpeed=0.01,isCarExcepted=true,},
  { tag = 14,type = RoleType.robject,frame = "question_red.png",pos = Vec2(8,13), offset=Vec2(0, 10), isSolid=false,allowGravity=false,onCheckTile=false, visible=false},
  { tag = 15,type = RoleType.robject,frame = "tlight1.png",pos = Vec2(47,15), offset=Vec2(18, -32.5), isSolid=false,allowGravity=false,onCheckTile=false, allowTouch = true, frameIndex=1,touchTimes=0},

  { tag = 16,type = RoleType.robject, tile=Tile.charBlock,pos = Vec2(43,11),char={ text='D',font=Font.char, rangeStart='0',rangeEnd='9',},knocks=knockChar, },
  { tag = 17,type = RoleType.robject, tile=Tile.charBlock,pos = Vec2(44,11),char={ text='I',font=Font.char, rangeStart='0',rangeEnd='9',},knocks=knockChar,  },
  { tag = 18,type = RoleType.robject, tile=Tile.charBlock,pos = Vec2(45,11),char={ text='C',font=Font.char, rangeStart='0',rangeEnd='9',},knocks=knockChar, },
  { tag = 19,type = RoleType.robject, tile=Tile.charBlock,pos = Vec2(46,11),char={ text='K',font=Font.char, rangeStart='0',rangeEnd='9',},knocks=knockChar, },

  --礼品 poo 
  { tag = 20,type = RoleType.robject, frame="gift_orange.png",pos = Vec2(52,7), offset=Vec2(30,0), visible=true, allowGravity=false,allowPush=CollisionDirection.intersected,},
  --zzi
  { tag = 21,type = RoleType.npc, frame="zzi1.png",pos = Vec2(72,11), step=Vec2(2,2), face=MovingDirection.toLeft, },
  --wb
  { tag = 22,type = RoleType.npc, frame="wb1.png", framePrefix="wb",pos = Vec2(70,11), face=MovingDirection.toLeft,},
  { tag = 23,type = RoleType.robject, allowGravity=true, isSolid=false, insetX=25, file=Folder.texture.."map.png",pos = Vec2(70,11), visible = false,   face = MovingDirection.toLeft,},


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
    --打开礼品
    if (Joystick.ref:getTouchedPad(2)) then 
      Role[RoleIndex.gift].frame = "poo.png"
      Role[RoleIndex.gift].ref:setFrame("poo.png")
      Role[RoleIndex.gift].ref.isHarmful = true
      Role[RoleIndex.gift].ref.isSolid=false
      Role[RoleIndex.gift].sound = Sound.manDied
    end
  end
end

preload = function()
  --根据语言载入对话,cpp动态写入语言，必须在函数内执行
  require("dialog".."4-"..Language)
  PopupInfo = require("popup"..Language)
  --预先载入公共事件或属性，比如Role里面的事件
  preloadStage()

  local iter = 1
  Role[iter].fsm = Fsm:new(Role[iter],State4.Player.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],State4.RObject.Routine,State4.RObject.Cloud) iter = iter + 1 --云彩
  --混蛋
  Role[iter].fsm = Fsm:new(Role[iter],NpcStates.Routine,State4.Npc.Bastard) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],NpcStates.Routine,State4.Npc.Bastard) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],NpcStates.Routine,State4.Npc.Bastard) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],NpcStates.Routine,State4.Npc.Bastard) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],NpcStates.Routine,State4.Npc.Bastard) iter = iter + 1
  --广告
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State4.RObject.Ads) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State4.RObject.Ads) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State4.RObject.Ads) iter = iter + 1
  --car2
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  --question red
  Role[iter].fsm = Fsm:new(Role[iter],nil,State4.RObject.QuestionRed) iter = iter + 1
  --红绿灯
  Role[iter].fsm = Fsm:new(Role[iter],nil,State4.RObject.TrafficLight) iter = iter + 1
  --密码砖头
  Role[iter].fsm = Fsm:new(Role[iter],RObjectStates.Knocked,State4.RObject.BlockChar) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],RObjectStates.Knocked,State4.RObject.BlockChar) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],RObjectStates.Knocked,State4.RObject.BlockChar) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],RObjectStates.Knocked,State4.RObject.BlockChar) iter = iter + 1
  --礼品
  Role[iter].fsm = Fsm:new(Role[iter],nil,State4.RObject.GiftOrange) iter = iter + 1
  --王八
  Role[iter].fsm = Fsm:new(Role[iter],nil,State4.Npc.Zzi) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State4.Npc.Wangba) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1

  --触发器
  iter = 1
  --drip
  Triggers[iter].fsm = Fsm:new(Triggers[iter],nil,State4.Triggers.Drip)  iter = iter + 1   
  --spawn car 1
  --检查站
  if (Checkpoint.id == 0 ) then
    CommonStates.Spawn.roles = function()
      local  role = {type = RoleType.npc,frame =  "car1.png", frameIndexes=FrameTemplate.car,pos= Vec2(0,17),  direction=MovingDirection.toRight,step=Vec2(3.5,3.5), allowGravity=false, onCheckTile=true, insetX=15, frameSpeed=0.01,}

      role.fsm=Fsm:new(role,nil,CommonStates.Move) 
      return role
    end  
    CommonStates.Spawn.setCount(1,4.5)
    Triggers[iter].fsm = Fsm:new(Triggers[iter],nil,CommonStates.Spawn) iter = iter + 1 
  else
    Triggers[iter].fsm = Fsm:new(Triggers[iter],nil,State4.Triggers.SpawnCars)   iter = iter + 1 
  end

  --线段几何碰撞
  Triggers[iter].fsm = Fsm:new(Triggers[iter],nil,nil)  iter = iter + 1  
  Triggers[iter].fsm = Fsm:new(Triggers[iter],nil,nil)  iter = iter + 1 
  Triggers[iter].fsm = Fsm:new(Triggers[iter],nil,nil)  iter = iter + 1 
  --spawn car2
  Triggers[iter].fsm = Fsm:new(Triggers[iter],nil,nil)  iter = iter + 1 
  Triggers[iter].fsm = Fsm:new(Triggers[iter],nil,nil)  iter = iter + 1 


end
-- cpp初始化调用，
initiate = function()
  
  Ref.stage:loadCurtain({type = CurtainType.bilateral, image={"background/leftrage.png","background/rightdick.png"},  delay = 1, zorder = ZOrder.dropCurtain})
  Resh.playMusic(Music.Rhapsody)

  --checkpoint
  if (Checkpoint.id > 0 ) then
    Role[1].ref:spawn(Checkpoint.position,Vec2.ZERO)
    Role[RoleIndex.ad1].isDisposed = true 
    Role[RoleIndex.ad2].isDisposed   = true 
    Role[RoleIndex.ad3].isDisposed   = true  
    Role[RoleIndex.kid].isDisposed = true 
    Role[RoleIndex.man].isDisposed = true 
    Role[RoleIndex.woman].isDisposed = true 
    Role[RoleIndex.lady].isDisposed = true 
    Role[RoleIndex.cowboy].isDisposed = true 
    Role[RoleIndex.car2]. isDisposed = true 
    Role[RoleIndex.questionRed].isDisposed = true 
  end
  createStar(3,Vec2(62,12),ZOrder.robject) 
  createStar(11,Vec2(4,11),ZOrder.robject)
 
  globalInit()
end
 --  local size = Resh.getVisibleSize()
--循环调用
update = function()
 
    
  if (checkRetry()) then
    updateGlobal()
  end
end