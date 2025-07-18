local Telegram = require("telegram")
local StateMachine = { }


function StateMachine:new(e, gs, cs)
	assert(e~=nil,"entity cannot be null")
	--assert(gs~=nil,"ȫ���಻��Ϊ��")
	--assert(cs~=nil,"��ǰ״̬�಻��Ϊ��")
  
	local  obj = {}
  --表元素不能直接在表内访问，判断如果是数字在函数中访问
  if (type(e) == 'number') then
    obj.entity = Role[e]
  else
    obj.entity = e
  end
	obj.globalState = gs
  
	obj.currentState = cs
--  if (cs ~= nil and  cs.exit ~= nil) then
--    cs.enter(obj.entity)
--  end
	setmetatable(obj, {__index = StateMachine})
	return obj
end

function StateMachine:update()
	if (self.globalState~= nil and self.globalState.execute ~= nil) then
		self.globalState.execute(self.entity)
	end
	
	 if (self.currentState~= nil and self.currentState.execute ~= nil) then
	  self.currentState.execute(self.entity)
	end
end

function StateMachine:changeState(newState)
    self.prevState = self.currentState
		 --如果不为空才执行exit
		if (self.currentState ~= nil and self.currentState.exit ~= nil) then
			self.currentState.exit(self.entity)
		end
		
		self.currentState = newState
    --如果不为空才执行enter
		if (self.currentState ~= nil and self.currentState.enter ~= nil) then
			self.currentState.enter(self.entity)
		end
end
--全局
function StateMachine:changeGlobal(newState)
  
		 --如果不为空才执行exit
		if (self.globalState ~= nil and self.globalState.exit ~= nil) then
			self.globalState.exit(self.entity)
		end
		
		self.globalState = newState
    --如果不为空才执行enter
		if (self.globalState ~= nil and self.globalState.enter ~= nil) then
			self.globalState.enter(self.entity)
		end
end


function StateMachine:clearState()
  self:changeState(nil)
end
function StateMachine:clearGlobal()
  self.globalState = nil
end
function StateMachine:revertState()
		self:changeState(self.prevState)
end

function StateMachine:isInState(state)
		return state == self.currentState
end
--如果执行完后返回为true则停止在发送消�?
--否则继续迭代执行
function StateMachine:handleMessage(telegram)
  
--  if (telegram.dispatchTime > 0.0) then

--    return false
--  end
	if(nil~= self.currentState and nil ~= self.currentState.onMessage) then
		local msgResult = self.currentState.onMessage(self.entity,telegram)
		if (true == msgResult) then
			return true
		end
	end

	if (nil~= self.globalState and nil ~= self.globalState.onMessage) then
		local msgResult = self.globalState.onMessage(self.entity,telegram)
		if (true == msgResult) then
			return true
		end
	end

	return false
end

return StateMachine
 
