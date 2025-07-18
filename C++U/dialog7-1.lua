RObjectText =
{  
  helmet = "戴上死磕牌头盔\n 砖头瓦片变成灰" 
} 

Dialogue =
{
  --0 到 8
  { tag=2, text ="钢蛋先生你可真邪性，迪克大魔王叫我在这里等着你", font = Font.default, } ,
  { tag=1, text ="汪思甜，你嘴巴可真臭，迪克又给你喂大翔了吧！", font = Font.default },
  { tag=1, text ="有啥本事尽量使出来，别跟我来这个里根楞。", font = Font.default }, 
  { tag=2, text ="阁下武功高强，不知脑壳好使不? \n 所以本汪在这里给你出几道智力题,\n看看你能答对几个。", font = Font.default },
  { tag=2, text ="答对了有礼品，如果答错了也有“安慰奖”", font = Font.default },
  { tag=1, text ="好啊，我好歹也是小学本科学历，\n在劳白肝一中年年都考第2名！",font = Font.default },
  { tag=2, text ="那第一名是谁？",font = Font.default },
  { tag=1, text ="倒数第一不是我就行，你管那么多干什么！",font = Font.default },
  { tag=2, text ="闲话一会儿再续，我要开始出题了！",font = Font.default },
  { tag=1, text ="尽管放狗过来！！！",font = Font.default },
}

Question =
{
  {text=[[第1题：1 + 1 = ? 
A.2 B.1 C.22 D.11 ]], font = Font.default,key='A',   correct =  {text="真失败，竟然答对了 -_-", font=Font.default,}, 
    incorrect= {text="恭喜您，答错了！", font=Font.default,},},
  
  {text=[[第2题：
1+1+1+1+1
1+1+1+1+1
1+1*0+1=?
A.11 B.12 C.13 D.30]], font = Font.default,key='D', correct =  {text="真失败，竟然答对了 -_-", font=Font.default,}, incorrect= {text="就这个feel倍儿爽儿！", font=Font.default,},},

  {text=[[第3题：谁是这个星球最帅的人
A.汪思甜 B.迪克 C.史莱姆 D.钢蛋 ]], font = Font.default,key='A', correct =  {text="答对了：礼物来", font=Font.default,}, incorrect= {text="答错了！看来你也应该学怎么拍马屁。", font=Font.default,},},

  {text=[[第4题：在这个星球生活的人是？
A.长得像球一样的混蛋。
B.愚蠢、弱智、脑袋瓜有问题。
D.被迪克大魔王脑控，得斯德哥尔摩综合征的人 ]], font = Font.default,key='D', correct =  {text="Bingo，礼物又来了", font=Font.default,}, incorrect= {text="恭喜您，答错了！", font=Font.default,},},

  {text=[[第5题：你认为迪克大魔王是哪种物种的原型：
B.白垩纪时代的霸王龙
D.戴着绿帽子、全身紫色的大魔头
C.包括以上：乌龟XXX、老板、邪恶势力、骗子的代表！ ]], font = Font.default,key='C', correct =  {text="好样的，再来一个~~~", font=Font.default,}, incorrect= {text="恭喜您，答错了！", font=Font.default,},},

  {text=[[最后一题：
你认为250除了数字之外还有什么意思？
A.吉祥如意 B.地名 C.标识 D.时间 ]], font = Font.default,key='NoKey', correct =  {text="250", font=Font.default,}, incorrect= {text="恭喜您 二百五，答错了！", font=Font.default,},},
}
 