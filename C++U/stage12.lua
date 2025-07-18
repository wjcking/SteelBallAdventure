--测试状态机在里面
require("global")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local NpcStates = require("npcstates")
local Telegram = require("telegram")
local CommonStates = require("commonstates")
local State12= require("state12")
PopupInfo = nil
Resources =
{  
  { music = Music.TrialFormer }, 
}
Background =
{
  {file= Folder.background.."dusk.png",},
}
-- 地图
Atlas =
{
  {file = "map/map12.tmx" , view =  MapView.horizontal,  }  
}
Triggers =
{

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
  jump = "jump", 
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
  { file = "control/fire.png",pos = Vec2(ScreenWidth - 70 * 2.5, 45), name = CommandPad.fire, visible= false, }, 
}

RoleIndex =
{
  wangba=2,
  kid = 5,
  man = 6,
  woman = 7,
  lady = 8,
  cowboy = 9,
  block1=3,
  block2=4,
  question=11,
  block3=13,
  block4=14,
}
local knockChar =
{ 
  {type = KnockAction.popup,direction=CollisionDirection.atBottom,limitedTimes=-1,},
}
local FrameNpc=
{
  {type = FrameStyle.idle,  rangeStart =14, rangeEnd =14,   },
  {type = FrameStyle.faceUP,  rangeStart =6, rangeEnd =6,},
  {type = FrameStyle.faceDown,  rangeStart =10, rangeEnd =10,},
  {type = FrameStyle.walkX,  rangeStart =1, rangeEnd =2,},
  {type = FrameStyle.walkUP,  rangeStart =6, rangeEnd =8,},
  {type = FrameStyle.walkDown,  rangeStart =10, rangeEnd =12,},
  {type = FrameStyle.fire,  rangeStart =5, rangeEnd =5,},
  {type = FrameStyle.fireUP,  rangeStart =9, rangeEnd =9,},
  {type = FrameStyle.fireDown,  rangeStart =13, rangeEnd =13,},
  {type = FrameStyle.jump,  rangeStart =3, rangeEnd =4,},
  {type = FrameStyle.hurt,  rangeStart =14, rangeEnd =14,},
  {type = FrameStyle.die,  rangeStart =15, rangeEnd =19,},
}
local gunDeadshot = { count=-1,  sound=Folder.sound.."poogun.wav",rate=0.1, frame="poo.png",project={type=ProjectType.deadshot,step=Vec2(5.08,5.08),allowGravity=false,target=1}}

Role = 
{
  { tag = 1,type = RoleType.player,frame = "player1.png",pos = Vec2(1,12),face=MovingDirection.toRight,hp=3,isReached=false, zorder=ZOrder.cover,},
  { tag = 2,type = RoleType.npc,frame = "wb1.png",pos = Vec2(15,8), offset=Vec2(0,0), allowGravity=false,face=MovingDirection.toLeft, frameIndexes=FrameTemplate.wb,weapon=gunDeadshot,hp=5,},


  { tag = 3,type = RoleType.robject,tile = Tile.charBlock,pos = Vec2(1,10) ,knocks=knockChar,offset=Vec2(0,10), onCheckTile=false, char={text='A',font=Font.char,},},
  { tag = 4,type = RoleType.robject,tile = Tile.charBlock,pos = Vec2(2,10) ,knocks=knockChar,offset=Vec2(0,10), onCheckTile=false,char={text='B',font=Font.char},},

  { tag = 5,type = RoleType.npc,frame = "kid1.png",  framePrefix="kid", frameIndexes=FrameNpc, pos = Vec2(4,7), isHarmful=false,face=MovingDirection.toRight, hp=1, isSolid=true, zorder=ZOrder.cover,},
  { tag = 6,type = RoleType.npc,frame = "man1.png",framePrefix="man",frameIndexes=FrameNpc,  pos = Vec2(6,7), isHarmful=false,face=MovingDirection.toRight ,hp=1,isSolid=true, zorder=ZOrder.cover,},
  { tag = 7,type = RoleType.npc,frame = "woman1.png",framePrefix="woman", frameIndexes=FrameNpc,pos = Vec2(8,7), isHarmful=false,face=MovingDirection.toLeft ,hp=1,isSolid=true,zorder=ZOrder.cover, },
  { tag = 8,type = RoleType.npc,frame = "cowboy1.png",framePrefix="cowboy", frameIndexes=FrameNpc,pos = Vec2(10,7), isHarmful=false,face=MovingDirection.toLeft, hp=1,isSolid=true,zorder=ZOrder.cover,},
  { tag = 9,type = RoleType.npc,frame = "lady1.png",framePrefix="lady", frameIndexes=FrameNpc,pos = Vec2(12,7), isHarmful=false,face=MovingDirection.toLeft ,hp=1,isSolid=true,zorder=ZOrder.cover,},
  { tag = 10,type = RoleType.robject,frame = "dialog_info.png",pos = Vec2(26,8), offset=Vec2(0,0), allowGravity=false,onCheckTile=true,allowTouch=true, allowDragX=true,allowDragY=true, allowPush=CollisionDirection.atRight, text=Hints.dialogBoard,zorder=ZOrder.cover,},
  { tag = 11,type = RoleType.robject,frame = "question_red.png",pos = Vec2(32,12), offset=Vec2(0,0), isSolid=false,allowNotify=true,allowGravity=false, },
  { tag = 12,type = RoleType.robject,frame = "spikebottom.png",pos = Vec2(13,12), isSolid=false,onCheckTile=false,allowGravity=false,sound=Sound.sting,isHarmful=true,step=Vec2(6,6),offset=Vec2(0,-12), },
  
  --候补
    { tag = 13,type = RoleType.robject,tile = Tile.charBlock,pos = Vec2(1,7) ,knocks=knockChar,offset=Vec2(0,10), onCheckTile=false, char={text='C',font=Font.char,},},
  { tag = 14,type = RoleType.robject,tile = Tile.charBlock,pos = Vec2(14,9) ,knocks=knockChar,offset=Vec2(0,0), onCheckTile=false,char={text='D',font=Font.char},},
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
  require("dialog".."12-"..Language)
  PopupInfo = require("popup"..Language)
  --预先载入公共事件或属性，比如Role里面的事件
  preloadStage()

  local iter = 1
  Role[iter].fsm = Fsm:new(Role[iter],State12.Player.Routine,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State12.RObject.BlockChoice) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State12.RObject.BlockChoice) iter = iter + 1

  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,nil) iter = iter + 1


  Role[iter].fsm = Fsm:new(Role[iter],nil,State12.RObject.DialogBoard) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State12.RObject.Question ) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State12.RObject.Spikes ) iter = iter + 1
  
    Role[iter].fsm = Fsm:new(Role[iter],nil,State12.RObject.BlockChoice) iter = iter + 1
  Role[iter].fsm = Fsm:new(Role[iter],nil,State12.RObject.BlockChoice) iter = iter + 1
  --触发器
  iter = 1 

end
-- cpp初始化调用，
initiate = function() 
  Ads.showBanner(Banner)
  Ref.stage:loadCurtain({type = CurtainType.bilateral, image={"background/leftrage.png","background/rightdick.png"},  delay = 1, zorder = ZOrder.dropCurtain})
  Resh.playMusic(Music.TrialFormer)
  State12.initialize()
  if (Checkpoint.id == 0) then
    Segment[1].fsm:changeState(State12.Dialogue.Dialog)
  elseif (Checkpoint.id==1) then
    Role[1].ref:spawn(Vec2(1,12),Vec2.ZERO)
    Role[RoleIndex.wangba].fsm:changeState(State12.Npc.DickQuestion)
    Role[RoleIndex.kid].fsm:changeState(State12.Npc.Bastard)
    Role[RoleIndex.man].fsm:changeState(State12.Npc.Bastard)
    Role[RoleIndex.woman].fsm:changeState(State12.Npc.Bastard)
    Role[RoleIndex.lady].fsm:changeState(State12.Npc.Bastard)
    Role[RoleIndex.cowboy].fsm:changeState(State12.Npc.Bastard)

  end
  createStar(8,Vec2(17,5),ZOrder.cover)
  globalInit()
end
--循环调用
update = function()

  if(checkRetry(2.5)) then
    updateGlobal()
  end
end