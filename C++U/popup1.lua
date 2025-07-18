require("global")
PopupText = { }
PopupText.life = "生命值:"
PopupText.tryAgain = "再来"
PopupText.grumble =
{
"钢蛋：\n炒fang价...",
"钢蛋：\n欺骗广大人民~~",
"钢蛋：\n忽悠群众~~",
"钢蛋：\n虐待小盆友~~",
"钢蛋：\n放大喇叭~~",
"迪克：\n饶了我吧、下次再也不敢了！", 
"迪克：\n英雄别打了，绕小人一命！", 
"迪克：\n大侠请高抬贵手要出人命了！",
}

PopupText.isDoneInfo = nil
PopupText.getDoneInfo =function()
  
  if PopupText.isDoneInfo ~= nil then
    return  PopupText.isDoneInfo
  end
  
    local info =
  {
    {text="我擦~",font=Font.default} ,
    {text="我挂啦~",font=Font.default} ,
    {text="真不敢相信，玩完了~",font=Font.default} ,
    {text= "~ (*+﹏+*)~",font=Font.default} ,
    {text="‘(*>﹏<*)′ ",font=Font.default} ,
    {text="啊啊啊呜~~~~~",font=Font.default} ,
  }

  if math.random(2) == 1 then
    PopupText.isDoneInfo = info[math.random(#info)]
  else
    PopupText.isDoneInfo = {}
  end

  return PopupText.isDoneInfo 
end
--对话框信息，注意多语言
local  PopupInfo = { }
local retrySize  =  { width=310,height=357}
--广告弹出频率
 

PopupInfo.createRetry = function()
  
 Ads.show(Checkpoint.life  == 5,Checkpoint.life % 4 == 0,Checkpoint.life == 1)
 
  Checkpoint.updateTrial()
  local isDick = Checkpoint.life == 0

  local buttonName = "retry"
  local visible = true
  local buttonText = PopupText.tryAgain
  local titles=
  {
    "你歇菜了","还是跪了","嗝儿屁了","~ (*+﹏+*)~","‘(*>﹏<*)′ "
  }
  local contents=
  {
    "广告多有不便，请您谅解~",
    "真正的英雄是不会因为愤怒\n退出游戏",
    "不怕百事不利，就怕灰心丧气。",
    "世上没有绝望的处境，\n只有对处境绝望的人。",
    "人有恒心万事成，人无恒心万事崩。",
    "含泪播种的人一定含泪微笑。",
  }
  if (isDick) then
    titles=
    {
      "快用球棒","我脸痒痒","别打我！",
    }
    contents=
    {
      "迪克大魔王:你就这点本事啊，\n钢蛋不像到像一个鸡蛋",
      "迪克大魔王:优美的动作\n 引无数英雄竞夭折",
      "迪克大魔王:哈哈哈，你完蛋了",
      "支持开发请点击广告\n",
    }
    --buttonText="重头再来"
    -- name="retry"
    visible = false
  end
  local function getFrame()
    if   isDick then
      return "dick1.png"
    elseif Checkpoint.life < 9-2-2  then
      return "fail3.png"
    elseif Checkpoint.life < 9-2 then
      return "fail2.png"
    elseif Checkpoint.life < 10  then
      return "fail1.png"
    end
  end
  local function getAnimation()
    if  isDick  then
      return Animate.jumpLaugh
    else
      return nil
    end
  end
  return  
  { 
    isCovered = true,
    autoClose = -1,
    name="retry",
    background =  {  file=Folder.popup.."retry.png", width = 500,height=500, sound=Folder.sound.."gameover1.wav"},
    items =
    {  
      { text= titles[math.random(#titles)], pos = Vec2(retrySize.width/2,retrySize.height - 20), name = "lbTitle",  font=Font.default},
      { text= PopupText.life..Checkpoint.life, pos = Vec2(retrySize.width/2,retrySize.height - 65), name = "lbLife",  font=Font.default},
      {  type=1, pos = Vec2(retrySize.width/2,retrySize.height/2),  name = "face",  frame=getFrame(), animate=getAnimation()},
      { text= contents[math.random(#contents)], pos =  Vec2(retrySize.width   * 0.5,70), name = "lbComment",  font=Font.retry},

      { text= buttonText,font=Font.default, file="control/popuplayer/button.png",pos =  Vec2(retrySize.width/2 ,10), name = buttonName,},
    }
  }
end
PopupInfo.recover = function()

  return  
  { 
    isCovered = true,
    autoClose = -1,
    name="retry",
    background =  {  file=Folder.popup.."retry.png", width = 500,height=500, sound=Folder.sound.."gameover1.wav"},
    items =
    {  
      { text= "你歇菜了", pos = Vec2(retrySize.width/2,retrySize.height - 20), name = "lbTitle",  font=Font.default},
      { text= "生命值：-250", pos = Vec2(retrySize.width/2,retrySize.height - 65), name = "lbLife",  font=Font.default},
      {  type=1, pos = Vec2(retrySize.width/2,retrySize.height/2),  name = "face",  frame="fail1.png",  },
      { text= "真正的英雄是不会因为愤怒、急躁\n而退出游戏", pos =  Vec2(retrySize.width   * 0.5,70), name = "lbComment",  font=Font.retry},

      { text= "复活",font=Font.default, file="control/popuplayer/button.png",pos =  Vec2(retrySize.width/2 ,10), name = "recover", },
    }
  }
end
PopupInfo.passed = function()
  
  Ads.show(Checkpoint.life % 3 == 0,Checkpoint.life % 5 == 0,true)

  local titles=
  {
    "恭喜过关"," O(∩_∩)O","└(^o^)┘","＝^ω^＝"
  }
  local contents=
  {
    "青海长云暗雪山，孤城遥望玉门关。\n黄沙百战穿金甲，不破楼兰终不还。",
    "无志者常立志，有志者事竟成。",
    "立志欲坚不欲锐，成功在久不在速。",
    "靠山山会倒，靠水水会流，\n靠自己永远不倒。",
  }
  local trialText =  "大获全胜"

  if  Checkpoint.trial > 0   then
    trialText =  "失败次数："..Checkpoint.trial
  end
  return  
  { 
    isCovered = true,
    autoClose = -1,
    name="passed",
    background =  {  file=Folder.popup.."retry.png", width = 500,height=500, sound=Folder.sound.."win.wav"},
    items =
    {  
      { text= titles[math.random(#titles)], pos = Vec2(retrySize.width/2,retrySize.height - 20), name = "lbTitle",  font=Font.medium},
      { text= trialText, pos = Vec2(retrySize.width/2,retrySize.height - 65), name = "lbLife",  font={18,Color.red,false},},
      {  type=1, pos = Vec2(retrySize.width/2,retrySize.height/2),  name = "face", frame="win.png", },
      { text= contents[math.random(#contents)], pos =  Vec2(retrySize.width * 0.5,70), name = "lbComment",  font=Font.retry},

      { text= "继续",font=Font.default, file="control/popuplayer/button.png",pos =  Vec2(retrySize.width/2 ,10), name = "continue", visible=true},
    }
  }
end
local achieveSize  =  { width=303,height=99}

PopupInfo.achieve = function(index)
  local achi = Achievements[index]
  return {
    index = index,
    name="achieve",
    autoClose = 6,
    zorder=10000,
    background =  {  file=Folder.popup.."bgachieve.png",  sound=Folder.sound.."achieved.wav",pos=Vec2(ScreenWidth/2,ScreenHeight/1.2)},
    items =
    {
      { type=1,font=Font.default,pos =  Vec2(achieveSize.width/5 , achieveSize.height/2), name = "image", file=Folder.achieve.."aimg_s.png" }, 
      { text= achi.text,font=Font.defaultWhite,pos =  Vec2(achieveSize.width/1.8 , achieveSize.height/2), name = "lbText",  font=Font.white16}, 
    }
  }
end
local objectSize  =  { width=322,height=222}
PopupInfo.createObject = function (image, desc)
  return  
  { 
    name="object",
    isCovered = true,
    autoClose = -1,
    background =  { file=Folder.popup.."objectacquired.png",  sound=Sound.objectAcquired },
    items =
    {   
      { type=1,font=Font.default,pos =  Vec2(objectSize.width/2 , objectSize.height/2), name = "image", file=image},
      { text=desc,font=Font.default,pos =  Vec2(objectSize.width/2 , objectSize.height/5), name = "lbText", },  
      { text= "明白了",font=Font.default, file=Folder.popup.."button.png",pos =  Vec2(objectSize.width/2 ,objectSize.height), name = "cancel",  },
    }
  }

end
local extendSize = { width=512, height=288}

PopupInfo.createExtend = function()

  return  
  { 
    isCovered = true,
    autoClose = -1,
    name="extend",
    background =  {  file=Folder.popup.."extend.png", width = extendSize.width,height=extendSize.height, sound=Sound.menuItem},
    items =
    {
      { text= "捐赠去广告",font=Font.default, file=Folder.popup.."button.png", pos =  Vec2(extendSize.width*0.2 ,extendSize.height), name = "btnDonate",  },
      { text= "点广告看攻略",font=Font.default, file=Folder.popup.."button.png", pos =  Vec2(extendSize.width *0.8 ,50), name = "btnAds",  },
      {  type=1, pos = Vec2(extendSize.width/4.4,extendSize.height * 0.5),  name = "adFace", frame="fail1.png", font=Font.default, },
      { text= "“攻略!先谢过了~”", font=Font.large,pos =  Vec2(extendSize.width/1.5,extendSize.height * 0.5),name="lbAds", },
    
      { text= "离开",font=Font.default, file=Folder.popup.."button.png", pos =  Vec2(extendSize.width/2 ,10), name = "btnCancel",  },
    }
  }
end
WalkThrough =
{
  { text=[[
[序幕攻略]
区域1：操纵英雄向上推动广告牌，触发绿色
按钮
区域2：选择“钢蛋的小手”用您的手指拖拽
广告牌拾取黄金球棒，在迪克身上画一个斜线
[获得成就]
序幕 一切都是刚刚开始
]], font=Font.default,},

  { text=[[
[本关攻略]
所有晃动的苹果不要理睬，直接走过去,不要碰问号，在最后砖块
上不要跳，否则会撞到云彩
[获得成就]
棉花糖 爱上我是你的缘
你是我的小苹果 苹果跟随到最后
小星星 一闪一闪亮晶晶 挂在天上放光明
]],font=Font.default,},

  { text=[[
[本关攻略]
区域1：
跳到最后一个绿苹果上面，踩住其取得红苹果，
跳到水管上面把红苹果放在下去。
在树上取得炸弹，然后把炸弹放到桥上
区域2：
敲击顺序：下2上2下1上1，然后从!上面!走
[获得成就]
小苹果 黄鼠狼给鸡拜年 没安好心
迪克的微笑1 上面不行就下面
]],font=Font.default,},

  { text=[[
[本关攻略]
区域1：
揭下3个广告宣传单，跟着吉普回去找到问号
区域2：
触摸红绿灯 跳上公共汽车，然后上屋顶，
敲击数字砖块 代码：2018
把礼品推到桥边上然后打开（一定要在桥边） 
[获得成就]
小广告 小广告清理者
2018 玩家今年要发
汪思甜 狗改不了吃屎 见屁股就舔]],font=Font.default,},
 
 { text=[[
[本关攻略]
区域1：顺序蓝红绿，推石头(中间两块石头可以推动)
压住上面两个按钮，去踩右下角按钮。
区域2：踩下红色按钮后，标有白色方块地方的砖块
可以推动，走出上去勾引双星刺然后绕回来走，
拦路倒刺消失。
区域3：触碰白色，勾引倒刺左右移动，起点处可以穿墙进去
[获得成就]
钢蛋跌倒记2 跌倒没商量
]],font=Font.default,},

 {text=[[
[本关攻略]
区域1：从最右面走，吃问号，然后从最左面走 吃问号。
区域2：拖拽中间的问号和右边的问号碰撞，
消灭所有史莱姆后，径直走向有刺的区域，倒刺会自动消失。
区域3：第一列小心钻石，第二列小心钻石后面的那两个金币，
第三列吃最左面2个钻石，然后吃剩余的。
[获得成就]
收敛不收手 该吃吃该喝喝 啥事别往心里搁]],font=Font.default,
},

{ text=[[
[本关攻略]
答案：ADADC，最后一个问题不要答！然后吃头盔，敲击砖块，
打开所有礼物，让狗狗去吃。
[获得成就]
三连跪 连续打错3道题
别添偶~ 不需要舔我屁股]],font=Font.default,},

{ text=[[
[本关攻略]
第1列：射击
第2列：射击
第3列：射击，踩上去小心气球会爆
第4列：先打死史莱姆，然后射气球，否则子弹跟踪
第5列：用手指触气球，别碰问号
第6列：合并问号，射击
最后：别面对乌龟，否则会发子弹
[获得成就]
迪克的微笑2 翔之地雷]],font=Font.default,},

{ text=[[
[本关攻略]
推墙70%后，操纵主角打天上多出来的小鸟，然后吃
推特图标攻击墙
[获得成就]
领导者 不用一枪一炮 
推墙 墙倒众人散]],font=Font.default,},

{ text=[[
[本关攻略]
区域1：
利用倒刺，刺杀毛毛虫，然后利用毛毛虫，
跳上小鸟取得最左侧水管上的钥匙，
踩着跳到过去
区域2：
勾引史莱姆加速撞击砖块喝下可乐，
然后用手拖拽对话框，主角从上面跳过去
区域3：
第二个水管可以拖拽，最后的水晶会躲闪
[获得成就]
销魂可乐 喝下销魂可乐 使您睡得更香
耍酷的铁蒺藜 绊倒你没商量]],font=Font.default,},

{ text=[[
[本关攻略]
区域1：
不要跳直接走上云梯，第二个云梯会下降赶紧跳到第三个
第四个会躲避，在走到第三个边上跳过去，
第五个上升不要往前面平台跳，
直到第四个移动后跳回到第四上面
区域2
弹簧力度一高一低， 最后水晶区域可以穿墙，
从墙后面借助弹簧跳上去拿水晶
[获得成就]
棉花糖2 我是天上一片云 你是地上一摊？
钢蛋跌倒记3 心急吃不上热豆腐
迪克的微笑3 上面不行就下面]],font=Font.default,},

{ text=[[
[本关攻略]
问题可答可不答，然后从混蛋身上跳过去，小心最后面倒刺，
对面下面的墙壁可以传过去，吃问号后，
上面的墙可以穿透， 用手点对话框推到最左侧，
借助其跳到上面墙里
射击大魔王。
[获得成就]
救世主 拯救所有人
英雄 至少拯救一位
坦坦荡荡做人 光明磊落做事
]],font=Font.default,},

{ text=[[
[本关攻略]
区域1：
中间砖块可以推动，推动其传送到右上面区域
压住按钮，然后从回到第一个区域再次踩红色按钮，
倒刺变慢等倒刺压住按钮后，
右下角区域触碰蓝色按钮。
区域2：
走下面穿过墙，进左侧漩涡，径直走向倒刺会自动消失，
慢慢移动到史莱姆旁边
让它移动后，取得钥匙，
再次进入左侧漩涡，按照路径走，但有最后一段不要走，
取得钥匙后，底部暗道打开
[获得成就]
迷魂阵 哎呀可算出来了
迪克的微笑3 水晶铁蒺藜
钢蛋跌倒记4 东边日出西边雨]],font=Font.default,},

{ text=[[
[本关攻略]
注：以下为红色的史莱姆
第一队：不用理睬，让绿色史莱姆跳过去
第二队：提前跳过去
第三队：小心背后偷袭，英雄跳2次
第四队：最后2个刺会跟着英雄跳，利用其攻击迪克，
小心后面哪个
[获得成就]
完美 你不能阻止我
小太阳 受不了迪克的翔]],font=Font.default,},

{ text=[[
[本关攻略]
区域1:
别拉绿色手闸，右侧有砖块可以推动压住左侧红色手闸
然后拉下绿色手闸，千万别跳坑里去，跳过去从中间下去
区域2:
拖动提示牌压住最右面的手闸，跳跃勾引史莱姆拉下中间手闸，
钢蛋跳跃拉下最左侧的手闸。
区域3：
吃左侧问号，射击右侧问号，把炸弹推到中间，
然后射击加速史莱姆使其能和炸弹摩擦
最后：
利用弹簧跳到第二区域在小星星前面有个砖块可以带走，
使其挡住大门
[获得成就]
史莱姆 开关
发疯的史莱姆 
合体 降落]],font=Font.default,},

{ text=[[
[本关攻略]
大魔王掉血到40%后，借助两个导弹，
从最左侧跳上去（墙壁可穿透看看上面哪2块砖不一样。）
[获得成就]
迪克复活 老王我又活过来了
好狗不挡道 狗改不了吃屎
背后被狗咬 给我5毛钱 我就往死里舔
流浪狗 汪思甜逃离！
]],font=Font.default,},

{ text=[[
[本关攻略]
隐藏星星：
水晶碎片1 第1关：区域3有墙壁可以穿进去，里面获得
水晶碎片2 第2关：射击完所有史莱姆后，回到区域1
水晶碎片3 第4关：显示史莱姆个数的工具栏里面，
借助提示框跳上去
水晶碎片4 第1关：在背后偷袭迪克魔王的隐藏区域（问号砖块）
跳跃拿去
第四章 水晶碎片4 第2关：进入最下面暗道后左侧4个
不同砖块的地方
[获得成就]
十二星 全部收集完毕
]],font=Font.default,},

{ text=[[
[闭幕攻略]
用手点云彩掉下物品。距离里英雄近，
才会掉物品
[获得成就]
健康100 收集100个红苹果
幸福100 收集100个青苹果
暴发户 收集100个金币
钻石大亨 收集100个钻石
全部回答正确 请接受礼品
感谢 观看制作人员 到最后
]],font=Font.default,},
}--end
return PopupInfo