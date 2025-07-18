local State = require("state")
local Telegram = require("telegram")
local CommonStates = { Triggers={},RObject={},}

CommonStates.Delay = State:new()
CommonStates.Delay.fsm=nil
CommonStates.Delay.state=nil
CommonStates.Delay.time =2.5
CommonStates.Delay.enter = function(delay)
  Delay.reset(DelayIndex.common)
end
CommonStates.Delay.execute = function(delay)
  if (CommonStates.Delay.fsm == nil or CommonStates.Delay.state == nil) then
    return
  end

  if (Delay.isTimeUp(DelayIndex.common, CommonStates.Delay.time)) then
    CommonStates.Delay.fsm:changeState(CommonStates.Delay.state) 
  end
end
CommonStates.Triggers.ScanOnce = State:new()
CommonStates.Triggers.ScanOnce.execute = function(trigger)

  if (trigger.isActive ~= nil and not trigger.isActive) then
    return
  end

  if (not  trigger.ref:scan(Role[1].ref)) then
    return
  end
  trigger.isActive = false 
  MsgDispatcher.dispatch(SEND_IMMEDIATELY,trigger,Role[1],MessageType.trigger)
end
CommonStates.Triggers.Collision = State:new()
CommonStates.Triggers.Collision.execute = function(trigger)

  -- Role[1].ref:checkLine(trigger.rangeStart, trigger.rangeEnd)

  -- MsgDispatcher.dispatch(SEND_IMMEDIATELY,trigger,Role[1],MessageType.trigger)
end
CommonStates.Triggers.Scanner = State:new()
CommonStates.Triggers.Scanner.execute = function(trigger)

  if (trigger.isActive ~= nil and not trigger.isActive) then
    return
  end

  if (not trigger.ref:scan(Role[1].ref)) then
    return
  end
  MsgDispatcher.dispatch(SEND_IMMEDIATELY,trigger,Role[1],MessageType.trigger)
end



CommonStates.RObject.Fall =  State:new() 
CommonStates.RObject.Fall.execute = function(object)

  local bound = object.ref:getBoundSelf()

  if (Role[1].ref:getBoundSelf().tilePosition.x == bound.tilePosition.x) then
    --关掉动画
    object.ref:stopAnimation()
    --声音fall播放一次
    if (not object.isFallen) then
      Resh.playSound(Sound.fall)
      object.isFallen = true
    end
    object.ref:setGravityOn()
  end
  --超过则销毁
  if (object.ref:getDistance().y >= ScreenHeight) then
    object.isDisposed = true
  end
end
--过界则销毁
--注意x，y 最好别是0否则会当场销毁
CommonStates.OutForDisposal =  State:new()  
CommonStates.OutForDisposal.execute = function(role)

  if (role.distance == nil) then
    role.distance = Vec2(ScreenWidth,ScreenHeight)
  end
  --超过则销毁
  --注意x，y 最好别是0否则会当场销毁
  if (role.ref:getDistance().x >= role.distance.x or
    role.ref:getDistance().y >=role.distance.y) then
    role.isDisposed = true
  end
end

CommonStates.Respawn =  State:new()  
CommonStates.Respawn.execute = function(role) 

  if (role.direction ~= nil) then
    role.ref:setDirection(role.direction)
  end
  if (role.distance == nil) then
    role.distance = Vec2(ScreenWidth,ScreenHeight)
  end
  --超过则销毁
  --注意x，y 最好别是0否则会当场销毁
  if (role.ref:getDistance().x >= role.distance.x or
    role.ref:getDistance().y >=role.distance.y) then 
    role.ref:resetVelocity()
    role.ref:respawn()
  end
end

--只能执行一个state，不是实例化，
--role必须设置成function 返回单个或数组，切记不要赋值变量
CommonStates.Spawn =  State:new()
CommonStates.Spawn.setCount = function(c,d)
  CommonStates.Spawn.count = c--无限创建 = -1
  CommonStates.Spawn.delay = d
  CommonStates.Spawn.number = 0 
end
CommonStates.Spawn.enter = function(trigger)
  --每次重置
  CommonStates.Spawn.number = 0
end
CommonStates.Spawn.execute = function(trigger)
  --注意role和起始位置可能是多个

--  if (CommonStates.Spawn.roles == nil) then
--    print("[CommonStates.Spawn]please set role")
--    return
--  end
  --是否无限创建
  local isLimited =  CommonStates.Spawn.count > 0
  --有限
  if (isLimited and CommonStates.Spawn.number >= CommonStates.Spawn.count) then
    print("[CommonStates.Spawn]already reached limitation")
    trigger.fsm:clearState()
    return
  end
-- 如果不是玩家，则什么都不做,前期只判断玩家
  if (Delay.isTimeUp(DelayIndex.spawn, CommonStates.Spawn.delay)) then
    --如果就一个
    if (#CommonStates.Spawn.roles() == 0) then
      createRole(CommonStates.Spawn.roles() )
    else
      for _,each in ipairs(CommonStates.Spawn.roles()) do
        createRole(each)
      end
    end
    --重置
    Delay.reset(DelayIndex.spawn)
    --不是无限个数则
    if (isLimited) then
      CommonStates.Spawn.number = CommonStates.Spawn.number + 1
    end
  end
end
---注意这里object，npc，player都可以
--出界以后就消除
CommonStates.Move =  State:new()
CommonStates.Move.execute  = function(role)
  if (role == nil or role.isDisposed) then
    return
  end
  if (role.direction == nil) then
    print("[CommonStates.Move]please set direction attribute")
    role.fsm:clearState()
    return
  end
  role.ref:setDirection(role.direction)
  local distance = role.ref:getDistance()
  --过界则销毁 
  if (role.distance == nil) then
    if ((distance.x > ScreenWidth + ScreenWidth  ) or distance.y >= ScreenHeight) then
      role.isDisposed = true
    end
  else
    if ((distance.x > role.distance.x  ) or distance.y >= role.distance.y) then
      role.isDisposed = true
    end
  end
end

CommonStates.Dodge = State:new()
CommonStates.Dodge.execute = function(object)

  if (object.direction == nil) then
    object.fsm:clearState()
    return
  end
  
  local distance = object.distance
  
  if (distance == nil) then
    distance =Vec2(ScreenWidth*0.135,ScreenHeight*0.135)
  end
  
  if (object.direction == MovingDirection.toLeft) then
    object.ref:setForward()
      if (object.ref:getDistance().x >= distance.x) then
        object.ref:stop()
        object.fsm:clearState()
      end
--  elseif (object.direction == MovingDirection.toLeft) then
--  elseif (object.direction == MovingDirection.toTop) then
--  elseif (object.direction == MovingDirection.toBottom) then
  end
end
return CommonStates