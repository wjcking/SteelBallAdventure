local Telegram = require("telegram")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local CommonStates = require("commonstates")
local State = require("state")

local State14 = { Player={}, Npc={},RObject={}, ProjectTile={},Triggers={}, Dialogue={},}

local WaveCounter = 1 
local disposing = 0 

local step = Vec2(1.5,0.5)
local offset=Vec2(0,0)
local RisingObjects = nil 
local ActionStyle=
{
  none = 1,
  jump = 2,
  charge=3,
  sneakyBack = 4,
  jumping=5,
}
local insetSlime =Vec2(10,2)
local function getMeter(text)
  return  {text=text,font=Font.large,offset=Vec2(0,40),}
end
--记录上一次云彩位置不刷新
local cloud1Position = nil
local cloud2Position = nil
local cloud3Position = nil
local cloud4Position = nil
local cloud5Position = nil
local dickLaugh = 1
--初始化
State14.initalize = function()
  RisingObjects =
  {
    --***第1波**
    { 
      {type=RoleType.npc, frame="bigslime_gray1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_gray",pos=Vec2(20,8),step=step,   distance=distance, offset=offset, insetObject=insetSlime},
      {type=RoleType.npc, frame="bigslime_gray1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_gray",pos=Vec2(26,8),step=step, distance=distance,offset=offset,insetObject=insetSlime},
      {type=RoleType.npc, frame="bigslime_gray1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_gray",pos=Vec2(32,8),step=step,  distance=distance,offset=offset,insetObject=insetSlime},
      {type=RoleType.npc, frame="bigslime_green1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_green",pos=Vec2(38,8),step=step,  distance=distance,offset=offset,insetObject=insetSlime, action=ActionStyle.jump},
    },
    --***第2波**
    {
      {type=RoleType.npc, frame="bigslime_green1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_green",pos=Vec2(20,8),step=step, distance=distance, offset=offset, insetObject=insetSlime,action=ActionStyle.jump},
      {type=RoleType.npc, frame="bigslime_green1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_green",pos=Vec2(26,8),step=step, distance=distance,offset=offset, insetObject=insetSlime, action=ActionStyle.jump},
      {type=RoleType.npc, frame="bigslime_green1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_green",pos=Vec2(32,8),step=step, distance=distance,offset=offset, insetObject=insetSlime,action=ActionStyle.jump}, 
      {type=RoleType.npc, frame="bigslime_gray1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_gray",pos=Vec2(38,8),step=step, distance=distance,offset=offset, insetObject=insetSlime,},
    },
    --***第3波**存档
    {
      {type=RoleType.npc, frame="bigslime_gray1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_gray",pos=Vec2(20,8),step=step, distance=distance,  insetObject=insetSlime,offset=offset, },
      {type=RoleType.npc, frame="bigslime_green1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_green",pos=Vec2(26,8),step=step, distance=distance,offset=offset,  insetObject=insetSlime,action=ActionStyle.jump},
      {type=RoleType.npc, frame="bigslime_gray1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_gray",pos=Vec2(32,8),step=step, distance=distance,offset=offset, insetObject=insetSlime, },
      {type=RoleType.npc, frame="bigslime_green1.png",frameIndexes=FrameTemplate.slime,framePrefix="bigslime_green",pos=Vec2(38,8),step=step, distance=distance,offset=offset, insetObject=insetSlime,action=ActionStyle.jump},
    },
    --***第4波**
    {
      {type=RoleType.npc, frame="crab_red.png",  pos=Vec2(20,8),step=step, sound=Sound.sting, action=ActionStyle.charge },
      {type=RoleType.npc, frame="crab_red.png", pos=Vec2(26,8),step=step,  sound=Sound.sting,  action=ActionStyle.charge},
      {type=RoleType.npc, frame="crab_red.png", pos=Vec2(32,8),step=step,  sound=Sound.sting,  action=ActionStyle.charge},
      {type=RoleType.npc, frame="crab_green.png",  pos=Vec2(38,8),step=step, sound=Sound.sting, },
    },
    --***第5波**
    {
      {type=RoleType.npc, frame="crab_green.png",  pos=Vec2(20,8),step=step, sound=Sound.sting, insetObject=insetSlime, },
      {type=RoleType.npc, frame="crab_red.png", pos=Vec2(26,8),step=step,  sound=Sound.sting, insetObject=insetSlime, action=ActionStyle.charge},
      {type=RoleType.npc, frame="crab_green.png", pos=Vec2(32,8),step=step,  sound=Sound.sting, insetObject=insetSlime, },
      {type=RoleType.npc, frame="crab_red.png",  pos=Vec2(38,8),step=step, sound=Sound.sting, insetObject=insetSlime,action=ActionStyle.charge},
    },
    --***第6波**存档
    {
      {type=RoleType.npc, frame="crab_green.png",  pos=Vec2(20,8),step=step, sound=Sound.sting,  },
      {type=RoleType.npc, frame="crab_green.png", pos=Vec2(26,8),step=step,  sound=Sound.sting,  },
      {type=RoleType.npc, frame="crab_green.png", pos=Vec2(32,8),step=step,  sound=Sound.sting,  },
      {type=RoleType.npc, frame="crab_red.png",  pos=Vec2(38,8),step=step, sound=Sound.sting, action=ActionStyle.charge},
    },
    --***第7波** sneaky
    {
      {type=RoleType.npc, frame="agnail_green2.png",  pos=Vec2(20,8),step=step, sound=Sound.sting, allowFlip=true, },
      {type=RoleType.npc, frame="agnail_green2.png", pos=Vec2(28,8),step=step,  sound=Sound.sting,allowFlip=true,  },
      {type=RoleType.npc, frame="agnail_red2.png", pos=Vec2(36,8),step=step,  sound=Sound.sting,allowFlip=true, action=ActionStyle.sneakyBack},
      {type=RoleType.npc, frame="agnail_red2.png",  pos=Vec2(44,8),step=step, sound=Sound.sting,allowFlip=true,action=ActionStyle.sneakyBack},
    },
    {
      {type=RoleType.npc, frame="agnail_red2.png",  pos=Vec2(20,8),step=step, sound=Sound.sting, allowFlip=true,  action=ActionStyle.sneakyBack},
      {type=RoleType.npc, frame="agnail_green2.png", pos=Vec2(28,8),step=step,  sound=Sound.sting,allowFlip=true,  },
      {type=RoleType.npc, frame="agnail_red2.png", pos=Vec2(36,8),step=step,  sound=Sound.sting,allowFlip=true,action=ActionStyle.sneakyBack},
      {type=RoleType.npc, frame="agnail_green2.png",  pos=Vec2(44,8),step=step, sound=Sound.sting,allowFlip=true,},
    },
    {
      {type=RoleType.npc, frame="agnail_red2.png",  pos=Vec2(20,8),step=step, sound=Sound.sting, allowFlip=true,  action=ActionStyle.sneakyBack},
      {type=RoleType.npc, frame="agnail_green2.png", pos=Vec2(28,8),step=step,  sound=Sound.sting,allowFlip=true,  },
      {type=RoleType.npc, frame="agnail_red2.png", pos=Vec2(36,8),step=step,  sound=Sound.sting,allowFlip=true,action=ActionStyle.sneakyBack},
      {type=RoleType.npc, frame="agnail_red2.png",  pos=Vec2(44,8),step=step, sound=Sound.sting,allowFlip=true,action=ActionStyle.sneakyBack},
    },
    --第八波
    {
      {type=RoleType.npc, frame="spike.png", pos=Vec2(20,8),step=step, sound=Sound.sting, action=ActionStyle.none},
      {type=RoleType.npc, frame="spike.png", pos=Vec2(28,8),step=step, sound=Sound.sting, action=ActionStyle.none},
      {type=RoleType.npc, frame="spike.png", pos=Vec2(36,8),step=step, sound=Sound.sting, action=ActionStyle.none},
      {type=RoleType.npc, frame="spike.png", pos=Vec2(44,8),step=Vec2(1.0,1.0), sound=Sound.sting,action=ActionStyle.jumping},
    },
    {
      {type=RoleType.npc, frame="spike.png", pos=Vec2(0,9),step=Vec2(1.3,1.3), sound=Sound.sting, isSneaking=true, action=ActionStyle.jumping},
    },
  }
  dickLaugh = math.random(#DickLaugh)
  disposing = 0
  WaveCounter = 1
  if Checkpoint.id == 1 then
    Role[RoleIndex.cloudHints].fsm:changeState(State14.RObject.CloudHints)
  end
  if (Checkpoint.id < 2 ) then
    WaveCounter = 1
  elseif (Checkpoint.id==2) then
    WaveCounter = 4
    Role[RoleIndex.cloudHints].ref:registerText(Hints.cloudHints[2])
    --   Role[RoleIndex.cloudHints].fsm:changeState(State14.RObject.CloudHints)
  elseif (Checkpoint.id==3) then
    WaveCounter = 7
    Role[RoleIndex.cloudHints].ref:registerText(Hints.cloudHints[3])
    --  Role[RoleIndex.cloudHints].fsm:changeState(State14.RObject.CloudHints)
  elseif (Checkpoint.id==4) then
    WaveCounter = 10 
    Role[RoleIndex.cloudHints].ref:registerText(Hints.cloudHints[4])
    --  Role[RoleIndex.cloudHints].fsm:changeState(State14.RObject.CloudHints)
  end 

  if (cloud1Position ~= nil) then
    Role[RoleIndex.cloud1].ref:displace(cloud1Position)
    Role[RoleIndex.cloud2].ref:displace(cloud2Position)
    Role[RoleIndex.cloud3].ref:displace(cloud3Position)
    Role[RoleIndex.cloud4].ref:displace(cloud4Position)
    Role[RoleIndex.cloud5].ref:displace(cloud5Position)
  end
end
local function createRising()
  if (WaveCounter > #RisingObjects) then
    print("[createRising] WaveCounter out of boundary")
    return
  end
  local eachCount = #RisingObjects[WaveCounter]

  for i=1,eachCount,1 do
    RisingObjects[WaveCounter][i].fsm = Fsm:new(RisingObjects[WaveCounter][i],nil,State14.Npc.Slime)
    createRole(RisingObjects[WaveCounter][i])
  end
end
local function checkAllowRising()
  for _,npc in ipairs(Role) do
    --npc
    if (npc.type == RoleType.npc and not npc.isDisposed  and  npc.isDisposing == nil and not npc.isDispsoing ) then
      --释放判断 or sneaky
      if  npc.ref:getPositionX() < 0 or (npc.isSneaking~=nil and npc.ref:getPositionX() > ScreenWidth) then
        disposing = disposing + 1 
        npc.isDisposing = true 

      end

    end

  end

  return disposing == #RisingObjects[WaveCounter]
end


local function releaseRising()
  for _,npc in ipairs(Role) do
    if (npc.type == RoleType.npc and npc.isDisposing and npc.isExcepted == nil) then
      npc.isDisposed = true 
    end
  end
  clearDisposed()
end
State14.Player.Routine =  State:new()  
State14.Player.Routine.execute = function(player)
  if  player.ref:isDead() then
    player.ref.isFlippedX=false
    player.ref.moveStep=Vec2(3.5,3.5)
    player.ref:setBackward() 
  end
  PlayerStates.Routine.execute(player)

  if player.ref:getBodyStatus() == BodyStatus.dead or   player.ref:getBodyStatus() == BodyStatus.fall   then

    if (Delay.isTimeUp(DelayIndex.checkRetry,2)) then
      Ref.isRetried =true
      Ref.stage:prompt(PopupInfo.createRetry())
      Ref.popup:setSmack(Checkpoint.life == 0)
      player.fsm:clearGlobal()
    end
  end

end
State14.Player.Routine.onMessage = function(player, telegram)
  if (telegram.message == MessageType.gotShot) then
    Role[1].ref:gotHurt(1,Sound.hurt)
  end
end 


--对话
State14.Npc.Chasing = State:new()
State14.Npc.Chasing.execute = function(role)
  local distanceX = role.ref:getDistance().x
  role.ref:setForward()
  if role.tag == 1 then
    role.ref:showHints(Hints.grumblePlayer)
    if (distanceX > 185) then
      role.ref:closeDialog()
      role.ref.moveStep=Vec2(1.5,1.5)
      role.ref:stop()
      --存档
      Checkpoint.id =  Checkpoint.id  + 1
      role.fsm:clearState()
    end
  elseif (role.tag == RoleIndex.wangba) then
    if (distanceX > 300) then
      role.ref:showHints(Hints.dickCommand)
    else
      role.ref:showHints(Hints.grumbleDick)
    end
  else

    if (distanceX > 350) then
      Role[RoleIndex.wangba].fsm:changeState(State14.Npc.Command)

      role.ref:stop()
      role.fsm:clearState()
    end
  end

end


State14.RObject.Respawn =  State:new()  
State14.RObject.Respawn.execute = function(role) 
  --记录上一次位置
  cloud1Position = Role[RoleIndex.cloud1].ref:getPosition()
  cloud2Position = Role[RoleIndex.cloud2].ref:getPosition()
  cloud3Position = Role[RoleIndex.cloud3].ref:getPosition()
  cloud4Position = Role[RoleIndex.cloud4].ref:getPosition()
  cloud5Position = Role[RoleIndex.cloud5].ref:getPosition()
  if (role.direction ~= nil) then
    role.ref:setDirection(role.direction)
  end
  if (role.distance == nil) then
    role.distance = Vec2(ScreenWidth*1.2,ScreenHeight)
  end
  --超过则销毁
  --注意x，y 最好别是0否则会当场销毁
  if (role.ref:getDistance().x >= role.distance.x or
    role.ref:getDistance().y >=role.distance.y) then 
    role.ref:resetVelocity()
    role.ref:respawn()
  end
end
State14.RObject.RoadSign =  State:new()  
State14.RObject.RoadSign.execute = function(object) 
  if (object.direction == MovingDirection.stayStill) then
    return
  end
  object.ref:setDirection(object.direction)
  if  object.ref:getPositionX() < 0  then
    object.ref:stop()
    object.direction = MovingDirection.stayStill
    object.ref:resetVelocity()
    object.ref:respawn()
  end
end
--命令fireOn
State14.Npc.Command = State:new()
State14.Npc.Command.enter = function(npc) 
  npc.ref:closeDialog()
  npc.ref:stop()
  createRising() 
  Role[RoleIndex.cloudHints].fsm:changeState(State14.RObject.CloudHints)
  Role[RoleIndex.roadSign].ref:registerText(getMeter(WaveCounter))
  Role[RoleIndex.roadSign].direction = MovingDirection.toLeft
end
State14.Npc.Command.execute = function(npc)

  if (npc.ref:isDead()) then
    --不死成就
    if Checkpoint.trial == 0 then
      Ref.stage:promptAchieve(AchieveIndex.Unstoppable,PopupInfo.achieve(AchieveIndex.Unstoppable))
    end
    Ref.stage:prompt(PopupInfo.passed())
    npc.fsm:clearState()
  end
  if (Role[1].ref:isDead()) then 
    npc.ref:showHints(DickLaugh[dickLaugh])
    -- npc.fsm:clearState()
    return
  end
  if  math.random(1,20) <= 5 then
    npc.ref:getWeapon():setFireOn()
  else
    npc.ref:getWeapon():setFireOff()
  end

  --到头后，没了
  if (WaveCounter > #RisingObjects) then
    print("[State14.Npc.Command   WaveCounter out of boundary or player is dead]")
    Role[RoleIndex.sun].fsm:changeState(State14.RObject.Sun)
    Role[RoleIndex.wangba].ref:getWeapon():shootAt(Vec2.ZERO)
    releaseRising()
    disposing=0
    npc.fsm:clearState()
    return
  end

  local allowFetch = checkAllowRising()
  --下一波
  if (allowFetch) then

    releaseRising()

    if ( WaveCounter == 3) then
      if Role[1].ref:isAlive() then
        Checkpoint.id = Checkpoint.id + 1 --3
        Role[RoleIndex.roadSign].ref:registerText(getMeter(WaveCounter + 1)) 
        Role[RoleIndex.roadSign].direction   = MovingDirection.toLeft
        Role[RoleIndex.cloudHints].direction = MovingDirection.toRight
      end
      Role[RoleIndex.cloudHints].ref:registerText(Hints.cloudHints[2])
    end

    if ( WaveCounter == 6) then
      if Role[1].ref:isAlive() then
        Checkpoint.id = Checkpoint.id + 1 --6
        Role[RoleIndex.roadSign].ref:registerText(getMeter(WaveCounter + 1))
        Role[RoleIndex.roadSign].direction   = MovingDirection.toLeft
        Role[RoleIndex.cloudHints].direction   = MovingDirection.toRight
      end
      Role[RoleIndex.cloudHints].ref:registerText(Hints.cloudHints[3])
    end
    if (  WaveCounter == 9) then
      if Role[1].ref:isAlive() then
        Checkpoint.id = Checkpoint.id + 1 --9
        Role[RoleIndex.roadSign].ref:registerText(getMeter(WaveCounter + 1))
        Role[RoleIndex.roadSign].direction   = MovingDirection.toLeft
        Role[RoleIndex.cloudHints].direction = MovingDirection.toRight
      end
      Role[RoleIndex.cloudHints].ref:registerText(Hints.cloudHints[4])
    end
    WaveCounter = WaveCounter + 1
    disposing=0
    createRising()
  end 

end
State14.Npc.Command.exit = function(npc)
  npc.ref:getWeapon():setFireOff()
  npc.ref:closeDialog()
end

--史莱姆动作
State14.Npc.Slime = State:new()
State14.Npc.Slime.execute = function(npc)
  if (npc.isDisposing) then
    npc.fsm:clearState()
    return
  end
  --嘲笑
  if WaveCounter < 4 and Role[1].ref:isDead()  then
    npc.ref:setFrameIndex(FrameStyle.idle,4,4)
    npc.ref:setFrameIndex(FrameStyle.walkX,4,4)
    npc.fsm:clearState()
  end
  --如果已经背后偷袭，则不在向后走而是向前
  if (nil == npc.isSneaking)  then
    npc.ref:setBackward()
  else
    npc.ref:setForward()
  end
--  --距离判断，节省资源
  if (npc.ref:getRange(Role[1].ref) < RangeType.rangeSensed and npc.action ~= ActionStyle.jumping) then 
    return
  end

  if (npc.action == ActionStyle.jump) then
    local boundPlayer=Role[1].ref:getBoundSelf().tilePosition
    local boundNpc =npc.ref:getBoundSelf().tilePosition
    npc.ref.onJump = boundPlayer.x == (boundNpc.x - 1)

  elseif (npc.action == ActionStyle.charge) then  
    local boundPlayer=Role[1].ref:getBoundSelf().tilePosition
    local boundNpc =npc.ref:getBoundSelf().tilePosition

    if (boundPlayer.x == (boundNpc.x - 3) )  then 
      npc.ref.moveStep = Vec2(6,6)
    end
  elseif (npc.action == ActionStyle.sneakyBack) then
    local boundPlayer=Role[1].ref:getBoundSelf().tilePosition
    local boundNpc =npc.ref:getBoundSelf().tilePosition
    if (boundNpc.x < boundPlayer.x and math.abs(boundNpc.x - boundPlayer.x) == 3)  then 
      npc.isSneaking=true
      npc.ref:setForward()
      npc.ref.moveStep = Vec2(6,6)
    end
  elseif (npc.action == ActionStyle.jumping) then
    local boundPlayer=Role[1].ref:getBoundSelf().tilePosition
    local boundNpc =npc.ref:getBoundSelf().tilePosition
    if (boundNpc.x >= boundPlayer.x) then
      npc.ref.onJump = Role[1].ref.onJump
    end

    -- 如果不是玩么都不做,前期只判断玩家
    npc.getTarget(npc,Role[RoleIndex.wangba],function(range,opponent)
        if (range < RangeType.rangeThreatened) then 
          return
        end
        local isCollided = npc.ref:checkObjectCollision(opponent.ref,false)
        if (isCollided) then 
          npc.ref.isVisible = false
          Role[RoleIndex.wangba].ref:gotHurt(250,Sound.wangbaHurt)
          Role[RoleIndex.wangba].ref:setAnimation(Animate.getToastHurt(-250))
          disposing = disposing + 1 
          npc.isDisposing = true 
        end
      end)
  end
end

State14.RObject.Sun = State:new()
State14.RObject.Sun.onMessage = function(object,telegram)
  if (telegram.message == MessageType.gotShot) then
    Ref.stage:promptAchieve(AchieveIndex.SunPoo,PopupInfo.achieve(AchieveIndex.SunPoo))
    object.ref:stopAnimation()
    object.ref.isHarmful=true
    object.ref.allowGravity=true
    object.ref:gotHurt(1,Sound.fall)
  end
end

State14.RObject.CloudHints = State:new()  
State14.RObject.CloudHints.execute = function(object) 
  if (object.direction == MovingDirection.stayStill) then
    return
  end

  object.ref:setDirection(object.direction)
  if  object.ref:getDistance().x > object.distance.x  then
    object.ref:stop()
    object.direction = MovingDirection.stayStill
    object.ref:resetVelocity()
    object.ref:respawn()
  end
end
return State14