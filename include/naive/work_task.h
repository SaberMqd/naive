#ifndef _NAIVE_WORK_TASK_H_
#define _NAIVE_WORK_TASK_H_

namespace naive {

	struct WorkTask {

		explicit WorkTask() {}

		virtual ~WorkTask() {}
		
		virtual bool Process() = 0;

	};

}

#endif // !_NAIVE_WORK_TASK_H_
