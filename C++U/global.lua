--放到里面执行，先拷贝文件，后载入Fsm，直接加载有问题
Fsm = require("fsm")
MsgDispatcher = require("msgdispatcher")
ScreenWidth = 854
ScreenHeight = 480
--cpp端调用
Language = 1
--是否允许绘画碰撞边框 debuge
AllowDraw = false
IsDebug = false
IsResetAchieve = false
--场景，对话框的引用
Ref =  {  stage = nil, }
TriggerType ={ rect =1,line =2,circle =3,}
Color= { black =0, white = 1, red=2,green=3,blue=4,gray=5,orange=6 }

local SlapCount = 0
CameraSlideOffset =  Vec2(0,-4)

Platform_Unknown= 0
Platform_IOS    = 1
Platform_Android= 2
Platform_Win32  = 3
Platform_Linux  = 5
Platform_Mac    = 8
Platform_WinRT  =13
--you know
--if Resh.getPlatform() == Platform_Win32 or Resh.getPlatform() == Platform_Mac then
--Resh.setNumber(Constant.User_LastStage,18) 
--end
Urls =
{
  Main = "",
  Twitter = "https://twitter.com/wjcking18/",
  Community =  "community",
  Steelball= "steelball", 
  QQ="",
  Paypal="",
}

--文件夹设置
Folder =
{
  map="map/",
  texture = "texture/",
  joystick = "control/",
  popup = "control/popuplayer/",
  achieve = "control/achieve/",
  background = "background/",
  music = "music/",
  sound = "sound/",
}
ZOrder = 
{
  cover = -1,
  background = 1,
  map = 3,
  role = 5,
  enemy = 4,
  robject = 4,
  projectTile = 4,
  dropCurtain =5,
  joyStick = 6,
  overlay = 7,
  curtain = 8,
  popupLayer = 8,
  achieve = 8,
  draw = 6,
}
--声音文件
Sound =
{
  none = "",
  broken = Folder.sound.."broke.wav" ,
  menuItem= Folder.sound.."menuitem.wav" ,
  achieved= Folder.sound.."achieved.wav" ,
  sting= Folder.sound.."sting.wav" ,
  spike= Folder.sound.."sting.wav" ,
  spring= Folder.sound.."spring.wav" ,
  switch= Folder.sound.."switch.wav" ,
  hitHead= Folder.sound.."hithead.wav",
  objectAcquired= Folder.sound.."objectacquired.wav",
  object= Folder.sound.."objectacquired.wav",
  gameOver= Folder.sound.."gameover1.wav",
  crystalCrushed= Folder.sound.."crystal_crushed.wav",
  manHurt1 = Folder.sound.."man_hurt1.wav",
  manDied = Folder.sound.."man_died.wav",
  womanHurt1=Folder.sound.."woman_hurt1.wav",
  wangbaLaugh = Folder.sound.."wangba_laugh.mp3",
  hitWangba = Folder.sound.."hithead.wav",
  hurtPoo = Folder.sound.."hurt_poo.wav",
  hurt = Folder.sound.."hurt_poo.wav",
  playerHurt = Folder.sound.."hurt_poo.wav",
  win = Folder.sound.."win.wav",
  enigma = Folder.sound.."enigma.mp3",
  wangbaHurt = Folder.sound.."wangba_hurt2.wav",
  question = Folder.sound.."question.mp3",
  sneaky = Folder.sound.."sneaky.wav",
  explosion = Folder.sound.."explosion.wav",
  tick = Folder.sound.."tick.mp3",
  fall = Folder.sound.."fall.wav",
  turnPage = Folder.sound.."turnpage.wav",
  hitChar = Folder.sound.."hitchar.wav",
  coin = Folder.sound.."coin.wav",
  button = Folder.sound.."button.wav",
  toygun = Folder.sound.."toygun.wav",
  poogun = Folder.sound.."poogun.wav",
  missile = Folder.sound.."missile.mp3",
  incorrect = Folder.sound.."incorrect.wav",
  blocked1 = Folder.sound.."blocked1.wav",
  blocked2 = Folder.sound.."blocked2.wav",
  howling = Folder.sound.."howling.mp3",
  balloonBroken = Folder.sound.."balloon_broken.mp3",
  teleport = Folder.sound.."teleport.wav",
  trick = Folder.sound.."trick.mp3",
  charge =Folder.sound.."charge.mp3",
  popup = Folder.sound.."popup.mp3",
  star = Folder.sound.."star.wav",
}
Music = 
{
  overWorld = Folder.music.."Overworld.mp3",
  dungeonBoss = Folder.music.."DungeonBoss.mp3",
  paraspritePolka = Folder.music.."ParaspritePolka.mp3",
  SaltyDitty = Folder.music.."SaltyDitty.mp3",
  AJourneyAwaits = Folder.music.."AJourneyAwaits.mp3",
  MarineradMandarin = Folder.music.."MarineradMandarin.mp3",
  Rhapsody = Folder.music.."Rhapsody.mp3",
  BitQuest = Folder.music.."BitQuest.mp3",
  BitShift = Folder.music.."BitShift.mp3",
  Retro = Folder.music.."Retro.mp3",
  RaymanNight = Folder.music.."RaymanNight.mp3",
  BubbleBobble = Folder.music.."BubbleBobble.mp3",
  BubbleBobble2 = Folder.music.."BubbleBobble2.mp3",
  Yoyo = Folder.music.."Yoyo.mp3",
  TrialFormer = Folder.music.."TrialFormer.mp3", 
  AdventureTime = Folder.music.."AdventureTime.mp3",
  LittleApple = Folder.music.."LittleApple.mp3",
  Boss = Folder.music.."Boss.mp3",
  Europe = Folder.music.."Europe.mp3",
}

Stars=
{
  "Star1",  "Star2", "Star3",  "Star4",  "Star5", "Star6",
  "Star7",  "Star8", "Star9",  "Star10",  "Star11", "Star12",
}
local ScreenSize = Resh.getVisibleSize()
Banner=
{  
  isActive =false, x = ScreenSize.width/2, y = ScreenSize.height*2,  height=150, width=600, interval = 30,
}

AchieveIndex =
{
  Beginning = 1,
--stage2
  Cloud=2, --棉花糖\n爱上我是你的缘"
  LittleApple1=3, --你是我的小苹果\n苹果跟随到最后"
--stage3
  LittleApple2=4, --小苹果\n黄鼠狼给鸡拜年\n没安好心"
  DickSmile1=5, --迪克的微笑1\n上面不行就下面"
  Star=6, --小星星\n一闪一闪亮晶晶\n挂在天上放光明"
--stage4 ******************
  Ads=7, --小广告\n小广告清理者"
  Fortune2018=8, --2018\n玩家今年要发"
  Zzi=9, --汪思甜\n狗改不了吃屎\n见屁股就舔"
--stage5
  PlayerFall1=10, --钢蛋跌倒记1\n跌倒没商量"
--stage6
  PlayerFall2=11, --钢蛋跌倒记2\n敞开胸怀，请你过来"
  Corrupt=12, --收敛不收手\n该吃吃该喝喝\n啥事别往心里搁"
--stage7******************
  WrongAnswers=13, --三连跪\n连续打错3道题"
  ZziCollided=14, --别添偶~\n不需要舔我屁股"
--stage8
  DickSmile2=15, --迪克的微笑2\n翔之地雷"
--stage9
  Commander=16, --鼓舞士气\n啸鹰计划\n"
  BreakWall=17, --推墙\n墙倒众人散"
--stage10
  Drink=18, --销魂可乐\n喝下销魂可乐\n使您睡得更香"
--******************
  Agnail=19, --耍酷的铁蒺藜\n绊倒你没商量"
--stage11
  Cloud2=20, --棉花糖2\n我是天上一片云\n你是地上一摊？"
  PlayerFall3=21, --钢蛋跌倒记3\n心急吃不上热豆腐"
  DickSmile3=22, --迪克的微笑3\n上面不行就下面"
--stage12
  Savior=23, --救世主\n拯救所有人"
  Hero=24, --英雄救美\n至少拯救一位\n女同志"
  --******************
  SneakyShoot=25, --坦坦荡荡做人\n光明磊落做事"
--stage13
  Formation=26, --迷魂阵\n哎呀可算出来了"
  DickSmile4=27, --迪克的微笑4\n水晶铁蒺藜"
  PlayerFall4=28, --钢蛋跌倒记4\n东边日出西边雨"
--stage14
  Unstoppable=29, --完美\n你不能阻止我"
  SunPoo=30, --小太阳\n受不了迪克的翔"
--stage15******************
  SlimeSwitch=31, --史莱姆\n开关"
  CrazySlime=32, --发疯的史莱姆\n"

  FallTogether=33, --合体\n降落"
--stage16
  DickRecover=34, --迪克复活\n老王我又活过来了"
  ZziCharge=35, --好狗不挡道\n狗改不了吃屎"
  ZziSneak=36, --背后被狗咬\n给我5毛钱\n我就往死里舔"
  --******************
  ZziRunWay=37, --迪克\n"
--stage17
  Star12=38, --十二星\n全部收集完毕"
--stage18
  Healthy=39, --健康100\n收集100个红苹果"
  Happiness=40, --幸福100\n收集100个青苹果"
  Rich=41, --暴发户\n收集100个金币"
  Monopoly=42, --钻石大亨\n收集100个钻石"
  --候补
  GotoHell=43,
  Final=44,
  Loser=45,
  Loser250=46,
  Slap50=47,
  Slap250=48,
}
--预先加载的资源
PreloadFiles =
{
  {sound= Sound.star },
  {sound= Sound.button },
  {sound= Sound.menuItem },
  {sound= Sound.achieved },
  {sound= Sound.string },
  {sound= Sound.switch },
  {sound= Sound.manDied },
  {sound= Sound.manHurt1 },
  {sound= Sound.womanHurt1 },
  {sound= Sound.hitHead },
  {sound= Sound.objectAcquired },
  {sound= Sound.gameOver },
  {sound= Sound.enigma },
  {sound= Sound.question },
  {sound= Sound.playerHurt },
  {sound= Sound.sneaky },
  {sound= Sound.explosion },
  {sound= Sound.tick },
  {sound= Sound.broken },
  {sound= Sound.fall },
  {sound= Sound.coin },
  {sound= Sound.trick },
  {sound= Sound.charge },
  {image=""},
  {frame="",rect="",name},
  {plist = Folder.texture.."role"},--不要加扩展名.png
  {plist = Folder.texture.."robject"},--不要加扩展名.png
  {plist = Folder.texture.."face"},--不要加扩展名.png
  {plist = Folder.texture.."scenery"},--不要加扩展名.png
  {plist = Folder.texture.."controls"},--不要加扩展名.png
}
Gid =
{
  brick= 37,
  spike=67,
  spikeBottom = 68,
  spikeTop = 69, 
  concreteTop = 5,
  concreteMid = 17,
  brickRed=52,
}
GidFlat =
{
  spikeBackRed = 168,
  spikeRed = 170,
  blockWall = 134,
  pad = 166,
  wallWhite = 144,
}
Tile =
{
  brickRed = Vec2(3,4),
  grayBlock = Vec2(2,5),
  charBlock = Vec2(0,6),
  grayBrick = Vec2(0,3),
  yellowRect = Vec2(2,3),
  blockRed = Vec2(4,5),
  spikeLeft = Vec2(9,5),
  spikeTop = Vec2(8,5), 
  flatSpikeRed = Vec2(4,15),
}
TileFlat =
{ 
  spikeLeft = Vec2(3,16),
  spikeTop = Vec2(2,16), 
  brick = Vec2(0,12),
  blockWall = Vec2(1,12), 
  wallWhite = Vec2(0,13),
  spikeRed = Vec2(4,15),
}
DelayIndex =
{
  passed = 1,
  recover = 4,
  stageEnd = 5,
  dialogDelay = 7,
  dispatch = 65530,
  dispatchSlime = 65531,
  spawn = 65532,
  common = 65533,
  checkRetry = 65534,
}

Font =
{
  default= {18,Color.black,false},
  padText = {18,Color.black},
  curtain= {25,Color.white,false},
  retry= {16,Color.black,false},
  retryLife= {14,Color.red,false},
  black15= {15,Color.black,false},
  white16= {16,Color.white,false},
  red18= {18,Color.red,false},
  small={14,Color.black},
  large={24,Color.black},
  warn= {22,Color.red},
  medium={18,Color.black},
  green22={22,Color.green},
  toast = {17,Color.red,false}, 
  char = {24,Color.black},
}
--武器和发射物预设值，count为每个弹夹的数量
Weapon=
{
  gunObject= {  count=-1,  sound=Folder.sound.."fire.wav",rate=0.08, frame="bullet1.png",project={type=ProjectType.object, delay=3,range=Vec2(100,100),step=Vec2(2,2)}},
  gunPoo= {  count=-1,  sound=Folder.sound.."fire.wav",rate=0.08, frame="poo.png",project={type=ProjectType.object, delay=3,range=Vec2(100,100),step=Vec2(2,2)}},
  gunFootboard = { count=15, sound=Folder.sound.."fire.wav",rate=15.5, frame="footboard.png",project={type=ProjectType.footboard, delay=5,allowDragY=true,dcExcept=CollisionDirection.atTop,offset=Vec2(50,50)}},
  gunDeadshot = { count=15,  sound=Folder.sound.."fire.wav",rate=15.5, frame="bullet1.png",project={type=ProjectType.trace,step=Vec2(0.08,0.08),target=1}},
}

--动画,有初始化使用的，也有刷帧时候用的
Animate= 
{
  getToast = function(text,font)
    return  { type= Anistyle.toast, text=text, sound=Folder.sound.."",font=font}
  end,
  getToastHurt = function(text,font)
    return  { type= Anistyle.toast, delay=0.5, text=text, sound=Folder.sound.."",font=font}
  end,
  getToastDelay = function(text,delay)
    return  { type= Anistyle.toast, delay=delay, text=text, sound=Sound.none,font=Font.default}
  end,
  getToastFrame  = function(frame)
    return  { type= Anistyle.toast, frame=frame, sound=Folder.sound..""}
  end,
  toastCoin  =    { type= Anistyle.toast, frame="coin.png", sound= Sound.coin, height=80,} ,
  toast = { type=1, text="", sound=Folder.sound..""},
  fade = { type = Anistyle.fade, delay=0.3, delayTime=0, sound=Folder.sound..""},
  fadeCrystal = { type = Anistyle.fade, delay=2.5, sound=Folder.sound..""},
  rollOut = { type = Anistyle.rollOut, sound=Folder.sound..""},
  explosion = { type = Anistyle.explosion, delay=1.5, sound=Sound.explosion},-- frame
  hovering = { type = Anistyle.rebound,delay=0.5,offset= Vec2(0,20)},
  hoverSun ={ type = Anistyle.rebound,delay=0,offset= Vec2(0,3), limitedTimes=-1},
  hoverCrystal = { type = Anistyle.rebound,delay=0,offset= Vec2(0,-10), limitedTimes=-1},
  hoverShake = { type = Anistyle.rebound, duration =0.05, delay=0,offset= Vec2(0,-4), limitedTimes=-1},
  reboundForever = {type = Anistyle.rebound,limitedTimes=-1, offset=Vec2(0,50), duration=1, durationBack=1, delay=0,delayBack=0},
  rebound = {type = Anistyle.rebound,limitedTimes=1, offset=Vec2(0,50), duration=0.2, durationBack=0.5, delay=0,delayBack=3, allowBack=true, sound=Folder.sound..""}, 
  reboundAgnail = {type = Anistyle.rebound,limitedTimes=1, offset=Vec2(0,45), duration=1, durationBack=1, delay=0,delayBack=1.5},
  reboundOnce= {type = Anistyle.rebound,limitedTimes=1, offset=Vec2(0,50), duration=1, durationBack=1, delay=0,delayBack=3, allowBack=false, sound=Folder.sound..""}, 
  rotate= { type = Anistyle.rotate,anchor=Vec2(0,0.5)},
  rotateSelf= { type = Anistyle.rotate,duration=3},
  jumpShake= { type = Anistyle.jump, height=3,duration=0.07},
  jumpLaugh= { type = Anistyle.jump, height=4,duration=0.25},

}
--开关 弹簧 按钮 
--name=前缀 direction 碰撞方向 limitedTimes=-1 无限，正整数 为次数
--如果 isSolid=true 请设置excepted ！[切记]！对手动作，抬起而不是自身,lua中player或enemy不要与此发生碰撞 
--noKey = -1;
--limitedFrame = -2;
--notCollided = -3;
--wrongDirection = -4;
--noFrame = -5;
--读去后CPP下标索引从0 开始而不是1

--弹簧 在顶部是重力控制 step=gravity
Spring =
{
  redBottom = { name="spring_red",direction=CollisionDirection.atTop,limitedTimes=-1, step=Vec2(0,10) },
  redTop = { name="spring_red_top",direction=CollisionDirection.atBottom,limitedTimes=-1, step=Vec2(0,-5) },
}
-- step 去的速率
-- stepBack 返回来的速率
-- range
-- delay 到达目的地后停留多少秒
FrameTemplate =
{
  playerRage=
  {
    {type = FrameStyle.idle,  rangeStart =1, rangeEnd =2, name="rage",},
--    {type = FrameStyle.faceUP,  rangeStart =6, rangeEnd =6,},
--    {type = FrameStyle.faceDown,  rangeStart =10, rangeEnd =10,},
    {type = FrameStyle.walkX,  rangeStart =1, rangeEnd =2,},
--    {type = FrameStyle.walkUP,  rangeStart =6, rangeEnd =8,},
--    {type = FrameStyle.walkDown,  rangeStart =10, rangeEnd =12,},
    {type = FrameStyle.fire,  rangeStart =5, rangeEnd =5,},
--    {type = FrameStyle.fireUP,  rangeStart =9, rangeEnd =9,},
--    {type = FrameStyle.fireDown,  rangeStart =13, rangeEnd =13,},
    {type = FrameStyle.jump,  rangeStart =3, rangeEnd =4,},
    {type = FrameStyle.hurt,  rangeStart =14, rangeEnd =14,},
    {type = FrameStyle.die,  rangeStart =15, rangeEnd =19,},
  },
  player=
  {
    {type = FrameStyle.idle,  rangeStart =1, rangeEnd =1, name="player",},
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
  },


  spring =
  {
    {type = FrameStyle.idle,  rangeStart =1, rangeEnd =5,name="spring_red"},
  },
  wangba =
  {
    {type = FrameStyle.idle,  rangeStart =1, rangeEnd =1, name="wangba",},
    {type = FrameStyle.faceUP,  rangeStart =1, rangeEnd =1,},
    {type = FrameStyle.faceDown,  rangeStart =1, rangeEnd =1,},
    {type = FrameStyle.walkX,  rangeStart =1, rangeEnd =1,},
    {type = FrameStyle.walkUP,  rangeStart =1, rangeEnd =1,},
    {type = FrameStyle.walkDown,  rangeStart =1, rangeEnd =1,},
    {type = FrameStyle.fire,  rangeStart =2, rangeEnd =2,},
    {type = FrameStyle.fireUP,  rangeStart =2, rangeEnd =2,},
    {type = FrameStyle.fireDown,  rangeStart =2, rangeEnd =2,},
    {type = FrameStyle.hurt,  rangeStart =3, rangeEnd =3,},
    {type = FrameStyle.die,  rangeStart =4, rangeEnd =4,},
  },

  cloud1 =
  {
    {type = FrameStyle.idle,  rangeStart =1, rangeEnd =1,name="cloud1",},
    {type = FrameStyle.hurt,  rangeStart =2, rangeEnd =2,},
    {type = FrameStyle.die,  rangeStart =2, rangeEnd =2,},
  },
  slime =
  {
    {type = FrameStyle.idle,  rangeStart =1, rangeEnd =1, },
    {type = FrameStyle.walkX,  rangeStart =1, rangeEnd =1,},
    {type = FrameStyle.walkUP,  rangeStart =1, rangeEnd =1,},
    {type = FrameStyle.walkDown,  rangeStart =1, rangeEnd =1,},
    {type = FrameStyle.hurt,  rangeStart =3, rangeEnd =3,},
    {type = FrameStyle.fire,  rangeStart =2, rangeEnd =2,},
    {type = FrameStyle.die,  rangeStart =3, rangeEnd =3,},
  },
  wb =
  {
    {type = FrameStyle.idle,  rangeStart =1, rangeEnd =1,name="wb",},
    {type = FrameStyle.walkX,  rangeStart =1, rangeEnd =1,},
    {type = FrameStyle.hurt,  rangeStart =2, rangeEnd =2,},
    {type = FrameStyle.fire,  rangeStart =3, rangeEnd =3,},
    {type = FrameStyle.die,  rangeStart =4, rangeEnd =9,},
  },
  bird=
  {
    {type = FrameStyle.idle,  rangeStart =1, rangeEnd =2,name="bird",},
    {type = FrameStyle.walkX,  rangeStart =1, rangeEnd =2,},
    {type = FrameStyle.hurt,  rangeStart =3, rangeEnd =3,}, 
    {type = FrameStyle.die,  rangeStart =3, rangeEnd =3,},
  },
  sun=
  {
    {type = FrameStyle.idle,  rangeStart =1, rangeEnd =1,name="sun",},
    {type = FrameStyle.hurt,  rangeStart =7, rangeEnd =7,},
    {type = FrameStyle.die,  rangeStart =7, rangeEnd =7,},
  },
  car=
  {
    {type = FrameStyle.idle,  rangeStart =1, rangeEnd =1,name="car",},
    {type = FrameStyle.walkX,  rangeStart =1, rangeEnd =5,}, 
  },
  jeep=
  {
    {type = FrameStyle.idle,  rangeStart =1, rangeEnd =1,name="jeep",},
    {type = FrameStyle.walkX,  rangeStart =1, rangeEnd =4,}, 
  },
  bus=
  {
    {type = FrameStyle.idle,  rangeStart =1, rangeEnd =1,name="bus",},
    {type = FrameStyle.walkX,  rangeStart =1, rangeEnd =4,}, 
  },
  pickup=
  {
    {type = FrameStyle.idle,  rangeStart =1, rangeEnd =3,name="pickup",},
    {type = FrameStyle.walkX,  rangeStart =1, rangeEnd =3,}, 
  },
}

local RObjectStates = require("robjectstates")
local PlayerStates = require("playerstates") 
local NpcStates = require("npcstates")
RoleModel=
{
  --子弹
  addProjectObject = function(ref)
    table.insert(Role,{ref = ref, type=ref:getType(), tag=ref:getTag(), isDisposed=false} )
    local id = #Role
    local shooterType = ref:getOwner():getType()

    Role[id].isDynamic = true
    Role[id].getOpponents = opponents
    Role[id].getPlayer = oppoPlayer
    Role[id].getTarget = oppoTarget
    if (shooterType == RoleType.player) then
      Role[id].fsm =Fsm:new(Role[id],PlayerStates.ProjectTile,nil) 
    elseif(shooterType == RoleType.npc) then
      Role[id].fsm =Fsm:new(Role[id],NpcStates.ProjectTile,nil) 
    end

  end,
  addCollapse = function(params)
    --注意collapse属性，cpp需要使用
    table.insert(Role,{type = params.ref:getType(),ref = params.ref, collapse=params,type=params.ref:getType(), tag=params.ref:getTag(), isDisposed=false } )

    Role[#Role].isDynamic = true
    Role[#Role].fsm =Fsm:new(Role[#Role],nil,RObjectStates.Collapse) 
    Role[#Role].getOpponents = opponents
    Role[#Role].getPlayer = oppoPlayer
    Role[#Role].getTarget = oppoTarget
  end,
--CPP一定要先加载完Role完毕后才能执行此方法，否则tag会覆盖原有的role元素无法执行
-- TiledMap::registerKnocks(const LuaRef & ref)
  addKnocks = function(params)
    table.insert(Role,{tag=params.ref:getTag(), ref = params.ref, type=params.ref:getType(), } )

    Role[#Role].isDynamic = true
    Role[#Role].fsm =Fsm:new(Role[#Role],nil,RObjectStates.Knocked)
    Role[#Role].getOpponents = opponents
    Role[#Role].getPlayer = oppoPlayer
    Role[#Role].getTarget = oppoTarget
  end,
  balloon ={type= RoleType.robject,file= Folder.texture.."balloon.png",pos=Vec2(12,15), step=Vec2(0.1,0.1), allowDragY=true,allowTouch=true,getOpponents = opponents , allowPush=CollisionDirection.intersected,},
--别忘了设置谁发出的owner
  bullet = {type = RoleType.projectTile,  offset=Vec2(0,0), step=Vec2(3,3), frame="bullet1.png", except={RoleType.player,RoleType.projectTile,RoleType.robject},isDisposed=nil},
  enemy =  {type = RoleType.projectTile, offset=Vec2(0,0), step=Vec2(3,3), frame="bullet2.png", delay = 1,  except={RoleType.player,RoleType.projectTile,RoleType.robject}},
}
--******************************************************
-- 以下是方法 
--*****************************************************

unload = function()
  PopupInfo = nil
  Resources = nil
  Background = nil
  Atlas = nil
  Triggers=nil
  Segment=nil
  DelayIndex=nil
  Collapse=nil
  CommandPad=nil
  Joystick=nil
  Joypad=nil
  RoleIndex=nil
  Role=nil
  callback=nil
  preload=nil
  initiate=nil
  update =nil 
  MsgDispatcher.clear() 
  DialogueSystem.close()
end
--当前地图索引，cpp修改后getStage根据此
mapIndex = 1
getStage = function()
  return Atlas[mapIndex].ref
end
--随机数
math.randomseed(tostring(os.time()):reverse():sub(1, 6))
-- 遍历角色人物 计算距离
opponents = function(roleSelf,funcOppo)
  for _,oppo in ipairs(Role) do
    --这里千万要注意，1） 如果不是自己，对手ref，对手没有释放
    if (
      roleSelf.tag ~= oppo.tag and 
      oppo.ref ~= nil and 
      (oppo.isDisposed == nil or not oppo.isDisposed) 
    ) 
    then
      funcOppo(roleSelf.ref:getRange(oppo.ref),oppo) 
    end
  end
end
----敌人对玩家  计算距离
oppoPlayer = function(roleSelf,funcOppo)
  --这里千万要注意，1） 如果不是自己，对手ref，对手没有释放
  local player = Role[1]
  if (roleSelf.tag ~= player.tag and player.ref ~= nil and (player.isDisposed == nil or not player.isDisposed)) then
    funcOppo(roleSelf.ref:getRange(player.ref),player) 
  end
end
----玩家或者敌人对单一角色  计算距离
oppoTarget = function(roleSelf, target, funcOppo)
  --这里千万要注意，1） 如果不是自己，对手ref，对手没有释放
  if (roleSelf.tag ~= target.tag and target.ref ~= nil and (target.isDisposed == nil or not target.isDisposed)) then
    funcOppo(roleSelf.ref:getRange(target.ref),target) 
  end
end
--菜单操作
callmenu =  function(className,funcName,params)
 
  if  className=="funcpads" and funcName == "funcpads" then

    -- 转换表
    local table = load("return "..params.table)()
    --打开链接
    if table ~= nil and table[params.tag].url ~= nil then
      Resh.openURL(table[params.tag].url)

    end
    if params.name == "reset"  then 
      Resh.playSound(Sound.turnPage)
      --清理成就和里面的失败次数
      Resh.reset(#Achievements)
      Resh.setNumber(Constant.User_Trial,0)     

      --清理星星
      for i=1,#Stars,1 do
        Resh.setNumber(Stars[i],0)      
      end
    end
  end
  if  className=="stage" and funcName == "menu" then
    --Resh.openURL(Urls.Steelball)
    Ref.stage:promptExtend(PopupInfo.createExtend())
  end
  --安卓
  if  className=="chapter" and funcName == "stage" and  math.random(20) == 1  and Resh.getPlatform() == Platform_Android then
    Ads.popup(true)
  end
end
--全局cpp call lua 方法
function globalCallFunc(className,funcName,params)


  --扇王八
  if  className == "popuplayer" and funcName =="ended" and Ref.popup:allowSmack() and Ref.popup:getName()=="retry" then
    -- Ref.popup:setButtonVisible("retry",Checkpoint.life > 2)
    local dickHead = Ref.popup:getChild("face")
    local rect = dickHead:getBoundingBox()
    local clap =  Ref.popup:isSlided(Vec2(0,10),rect)
    if (clap == SlidedResult.missed) then
      return
    end 
    dickHead:stopAnimation()
    Checkpoint.addLife()
    Resh.playSound(Sound.hitWangba) 
    dickHead:setFrame("dick2.png")
    dickHead.isFlippedX = clap == SlidedResult.right

    Ref.popup:setString("lbLife",PopupText.life..Checkpoint.life)
    Ref.popup:setString("lbComment",PopupText.grumble[math.random(#PopupText.grumble)])
    Ref.popup:setString("lbButton",PopupText.tryAgain)
    SlapCount = SlapCount + 1

    if SlapCount == 50 then
      Ref.stage:promptAchieve(AchieveIndex.Slap50,PopupInfo.achieve(AchieveIndex.Slap50))
    end
    if SlapCount == 250 then
      Ref.stage:promptAchieve(AchieveIndex.Slap250,PopupInfo.achieve(AchieveIndex.Slap250))
    end

  end
  if(Role[1].isDisposed) then
    return true
  end
  if (className == "collapse" and funcName =="collapse") then
    RoleModel.addCollapse(params)
  end
  if (className == "map" and funcName =="knock") then
    RoleModel.addKnocks(params)
  end


  return false
end
--预先载入公共事件或属性，比如Role里面的事件
function preloadStage()
  for _,each in ipairs(Role) do
    each.getOpponents = opponents
    each.getPlayer = oppoPlayer
    each.getTarget = oppoTarget
  end
  --初始化player frame
  if(Role[1].frameIndexes == nil) then
    Role[1].frameIndexes = FrameTemplate.player
  end
  --初始化对话fsm
  if (next(Segment) ~= nil ) then
    for _,each in ipairs(Segment) do
      each.fsm =  Fsm:new(each,nil,nil)
    end
  end 
end
function globalInit()
  --Joystick.ref:hideRocker()
  -- Ref.stage:setToolbar(false)
  local trial = Resh.getNumber(Constant.User_Trial)

  if (trial == 100) then
    Ref.stage:promptAchieve(AchieveIndex.Loser,PopupInfo.achieve(AchieveIndex.Loser))
  end

  if (trial == 250) then
    Ref.stage:promptAchieve(AchieveIndex.Loser250,PopupInfo.achieve(AchieveIndex.Loser250))
  end
end
function updateGlobal()

  --一定要放到update前面，否则碰撞有问题
  --目前只有player 别忘了tag属性
  --if (Role[roleID].type == RoleType.player) then
  for _,each in ipairs(Triggers) do
    if (each.tag~= nil and each.type == TriggerType.line) then
      Role[1].ref:checkLine(each.tag,each.rangeStart,each.rangeEnd)
    end
    if (each.fsm ~= nil) then
      each.fsm:update()
    end
  end
  --end
  --发送fsm延迟消息
  MsgDispatcher.dispatchDelayed()

--碰撞区域和对话
  if (next(Segment) ~= nil ) then
    for _,each in ipairs(Segment) do
      each.fsm:update()
    end
  end

  local isDisposed = false
  for index,each in ipairs(Role) do

    if (each.isDisposed ~= nil and each.isDisposed) then
      isDisposed = true
    else
      isDisposed = false
    end
    RoleManager.updateRole(each.tag,isDisposed)

    if ( not isDisposed and each.fsm ~= nil) then
      each.fsm:update()
    end
  end

end

--在callback中使用
function isFreezed() 
  if Role[1].ref == nil or  Role[1].ref:isFreezed()  then
    return true
  end
  --主角死亡
  if (Role[1].ref:getBodyStatus() == BodyStatus.dead ) then
    Role[1].ref:freeze()
    Role[1].onJump = false
    return true
  end
  return false
end
--检测是否弹出重来对话框
--如果主角释放了，就往下执行了
function checkRetry(delay)
  if (Role[1].ref:isDead()) then
    Ads.updateInterstitial()
  end
  --如果主角释放了
  if (nil == Ref.isRetried  and Role[1].isDisposed) then
    Ref.isRetried =true

    Ref.stage:prompt(PopupInfo.createRetry())
    return false
  end
  if (Role[1].ref:isDead()) then
    Role[1].ref:freeze()
    if (delay == nil) then
      delay =2
    end
    if (nil == Ref.isRetried and Delay.isTimeUp(DelayIndex.checkRetry,delay)) then
      Ref.isRetried =true
      Ref.stage:prompt(PopupInfo.createRetry()) 

    end
  end

  return true
end

--从Lua添加新的role到cpp，
function createRole(entity,fsm) 
  --添加到cpp端
  --如果没有roleModel不执行 
  if (nil == entity) then
    return
  end
  table.insert(Role,entity)
  --不等于空则负值
  if (nil ~= fsm ) then
    Role[#Role].fsm = fsm
  end

  Role[#Role].isDynamic = true
  Role[#Role].getOpponents = opponents
  Role[#Role].getPlayer = oppoPlayer
  Role[#Role].getTarget = oppoTarget
  --cpp
  RoleManager.appendRole(Role[#Role])

  return Role[#Role]
end
--从Lua添加新的role到cpp，
function createStar(starIndex,pos,zorder)

  if (Stars[starIndex] == nil or Resh.getNumber(Stars[starIndex] ) > 0) then
    return
  end
  --添加到cpp端
  --如果没有roleModel不执行 
  local entity={ type=RoleType.robject, onCheckTile=false,isSolid=false, zorder=zorder,pos=pos,frame="star.png",star=Stars[starIndex],animate= {type = Anistyle.rebound,limitedTimes=-1, offset=Vec2(0,2), duration=1, durationBack=1, delay=0,delayBack=0},}
  --不等于空则负值
  entity.fsm = Fsm:new(entity,nil,RObjectStates.Stars)
  entity.isDynamic = true
  entity.getOpponents = opponents
  entity.getPlayer = oppoPlayer
  entity.getTarget = oppoTarget 
  table.insert(Role,entity)
  --cpp
  RoleManager.appendRole(entity)

  return entity
end
function getStarCount()
  local starCount = 0
  for i=1,#Stars,1 do
    if  Resh.getNumber(Stars[i]) > 0  then
      starCount = starCount + 1
    end
  end
  return starCount
end
--清空动态生成的role
function clearDisposed()

  for i = #Role, 1, -1 do  
    if Role[i] ~= nil and Role[i].isDynamic~=nil and Role[i].isDynamic and Role[i].isDisposed~=nil and Role[i].isDisposed then
      --删除cpp端
      RoleManager.updateRole(Role[i].tag,true)
      table.remove(Role, i)
    end
  end
end
