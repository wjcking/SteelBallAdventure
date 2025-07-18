
local MsgDispatcher = {}
--注意先要打开库，然后实例化 metatable，否则有问题
local  QueueLibrary = require("queue")
local  Queue = nil
local Telegram = require("telegram")

--立即发送消息
local function discharge( receiver, telegram)
   if (nil == receiver.fsm) then
    print("[MsgDispatcher:discharge]fsm is null")
    return
  end
  if (not receiver.fsm:handleMessage(telegram)) then
   -- print("[MsgDispatcher:discharge]Message not handled")
  end
end


function MsgDispatcher.dispatch(delay,s,r,msg,info)
  local telegram = Telegram:new(delay,s,r,msg,info)

  if (delay <= 0.0) then
    discharge( r, telegram)
  else

    telegram.dispatchTime = Clock.getTick() + delay
    --在clear以后还要从新创建
    --如果不clear，队列还会在stage onExit后调用cpp 会产生错误
    if (Queue == nil) then
      Queue = QueueLibrary:new()
    end
    Queue.push(telegram)
  end

end

function MsgDispatcher.dispatchDelayed()
 
  if (Queue == nil or Queue:isEmpty() ) then
    return
  end
  local currentTime = Clock.getTick()
  local head =  Queue:peek()
  if (head.dispatchTime < currentTime  and head.dispatchTime > 0) then
    discharge(head.receiver,head)
    Queue.pop()
  end
end

function MsgDispatcher.clear()
  Queue = nil
end
return MsgDispatcher