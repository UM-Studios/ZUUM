  
"""
This is an example showing how to make the scheduler into a remotely accessible service.
It uses RPyC to set up a service through which the scheduler can be made to add, modify and remove
jobs.
To run, first install RPyC using pip. Then change the working directory to the ``rpc`` directory
and run it with ``python -m server``.
"""

import rpyc
from rpyc.utils.server import ThreadedServer

import apscheduler.jobstores.sqlalchemy
from apscheduler.schedulers.background import BackgroundScheduler
from apscheduler.triggers.cron import CronTrigger
from apscheduler.triggers.combining import OrTrigger
from apscheduler.triggers.interval import IntervalTrigger
from apscheduler.executors.pool import ThreadPoolExecutor
from appdata import appdata
import os
from datetime import datetime
from APSched import Task, Trigger

def serv_joinMeeting(task):
    args=task.args
    print(f'[{datetime.now().strftime("%Y-%m-%d %H:%M:%S")}] Ran {task.name}')
    os.startfile(f'zoommtg://zoom.us/join?{"&".join([arg+"="+args[arg] for arg in args if args[arg]])}')

class SchedulerService(rpyc.Service):
    def exposed_add_job(self, func, *args, **kwargs):
        job = scheduler.add_job(serv_joinMeeting, *args, **kwargs)
        fchanges = kwargs
        if 'args' in kwargs:
            fchanges['args'] = {arg: kwargs['args'][0].args[arg] for arg in kwargs['args'][0].args if kwargs['args'][0].args[arg] != ''}
        ffchanges = "\n".join("\t{}: {}".format(k, v) for k, v in fchanges.items())
        print(f'[{datetime.now().strftime("%Y-%m-%d %H:%M:%S")}] Added {job.name}:\n{ffchanges}')
        #print(f'[{datetime.now().strftime("%Y-%m-%d %H:%M:%S")}] Added {job.name}')
        return job

    def exposed_run_job(self, job_id, jobstore='default'):
        job = scheduler.get_job(job_id)
        trigger = job.trigger
        job.modify(trigger = OrTrigger([Trigger(day_of_week=6, hour=0, minute=0)]))
        job.modify(next_run_time = datetime.now())
        if not trigger.triggers:
            scheduler.pause_job(job_id, jobstore)
        job.modify(trigger = trigger)
        return scheduler.get_job(job_id)

    def exposed_modify_job(self, job_id, jobstore='default', **changes):
        job = scheduler.get_job(job_id)
        scheduler.modify_job(job_id, jobstore, **changes)
        fchanges = changes
        if 'args' in changes:
            fchanges['args'] = {arg: changes['args'][0].args[arg] for arg in changes['args'][0].args if changes['args'][0].args[arg] != ''}
        ffchanges = "\n".join("\t{}: {}".format(k, v) for k, v in fchanges.items())
        print(f'[{datetime.now().strftime("%Y-%m-%d %H:%M:%S")}] Modified {job.name}:\n{ffchanges}')
        return scheduler.get_job(job_id)

    def exposed_reschedule_job(self, job_id, jobstore='default', trigger=None, **trigger_args):
        return scheduler.reschedule_job(job_id, jobstore, trigger, **trigger_args)

    def exposed_pause_job(self, job_id, jobstore='default'):
        job = scheduler.get_job(job_id)
        scheduler.pause_job(job_id, jobstore)
        print(f'[{datetime.now().strftime("%Y-%m-%d %H:%M:%S")}] Disabled {job.name}')
        return scheduler.get_job(job_id)

    def exposed_resume_job(self, job_id, jobstore='default'):
        job = scheduler.get_job(job_id)
        scheduler.resume_job(job_id, jobstore)
        print(f'[{datetime.now().strftime("%Y-%m-%d %H:%M:%S")}] Enabled {job.name}')
        return scheduler.get_job(job_id)

    def exposed_remove_job(self, job_id, jobstore='default'):
        job = scheduler.get_job(job_id)
        scheduler.remove_job(job_id, jobstore)
        print(f'[{datetime.now().strftime("%Y-%m-%d %H:%M:%S")}] Removed {job.name}')
        return job

    def exposed_get_job(self, job_id):
        return scheduler.get_job(job_id)

    def exposed_get_jobs(self, jobstore='default'):
        return scheduler.get_jobs(jobstore)



if __name__ == '__main__':
    scheduler = BackgroundScheduler()
    url = f'sqlite:///{os.path.join(appdata, "meeting_data.db")}'
    scheduler.add_jobstore('sqlalchemy', url=url)
    scheduler.start()
    print(f'Importing from {os.path.join(appdata, "meeting_data.db")}')
    protocol_config = {'allow_all_attrs': True, "allow_pickle": True}
    server = ThreadedServer(SchedulerService, port=12345, protocol_config=protocol_config)
    try:
        print(f'\n[{datetime.now().strftime("%Y-%m-%d %H:%M:%S")}] Server Started')
        server.start()
    except (KeyboardInterrupt, SystemExit):
        pass
    finally:
        scheduler.shutdown()