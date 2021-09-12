[C.1.q2.1] Successful job execution:

Edit the batch script to specify that you want to run myprogramon 4 nodes (use the editcommand). Specify a duration that is sufficient for myprogramto complete successfully.
Submit this job to the batch scheduler and move time forward (using the sleepcommand) until after the job should have completed.
Check the content of the generated .outor .errfile produced by the job.

4 nodes, 20 hours
.out:
01/01 03:05:10: job submitted
01/01 03:05:10: job started
01/01 10:05:10: job successfully completed

Did the job complete successfully?
yes, it is completed
At what time did it complete?
at 10:05:10: job successfully completed.
Did it complete about when you thought it would?
yes, 2 + 20/4 = 7 hours

[C.1.q2.2] Failed job execution:

Now Submit a job to run myprogramon 6 nodes without enough requested time, so that it will certainly fail.
Once enough time has passed, check the content of the generated .outor .errfile.

6 nodes, 5 hours
.err:
01/01 00:05:17: job submitted
01/01 00:05:17: job started
01/01 05:05:22: job terminated due to time expiration
Did the job fail?
Yes
At what time did it fail?
01/01 05:05:22: job terminated due to time expiration

Did the job fail about when you thought it would?
Yes, with 6 nodes, myprogram runs in 2 + 20/6 = 5:20 > 5 hours



[C.1.q3.1] Inspecting the batch queue:

Submit a job to run myprogramon 25 nodes with enough requested time so that it will successfully complete.
Use squeueto inspect the state of the batch queue. You should see that your job is running.
Soon after, submit another job to run myprogramsuccessfully again (you can modify the same .slurmfile, or copy it), but using 10 nodes.
Use squeueto inspect the state of the batch queue. You should see that your second job is "stuck" in the queue, waiting for the first job to complete.

/$ squeue
JOBNAME   USER       NODES  START TIME      REQ TIME   STATUS
job_0     slurm_user 25     01/01 00:00:51  10:00:05   RUNNING
job_1     slurm_user 10     n/a             10:00:05   PENDING

Without advancing time, estimate the following:
the turnaround time of the first job.
2.8 hours

the turnaround time of the second job.
2.8 hours + 4 = 6 hours
Verify your answers to the above questions by advancing the clock and checking the content of the generated .out files.

/$ cat job_0.out
01/01 00:00:51: job submitted
01/01 00:00:51: job started
01/01 02:48:51: job successfully completed
/$ cat job_1.out
01/01 00:01:15: job submitted
01/01 02:48:51: job started
01/01 06:48:51: job successfully completed


[C.1.q4.1] Job submission and cancellation:

Submit a job to run myprogramon 16 nodes with enough requested time.
Soon after submission, inspect the state of the batch queue and answer the following questions:
How many jobs are currently pending?
14 jobs are pending
How many jobs are currently running?
2 jobs are running
Is your job pending or running? (your username is slurm_user)
my job is pending
Advance the time until your job has completed (using the sleepcommand). You will have to advance time by quite a lot. Imagine what it would be in the real world where, unlike in simulation, you cannot fast-forward time (if you can, contact us immediately!)
What was your job's wait time? (you can infer it based on the time of submission and the time of completion, since you know the execution time)
/$ cat job_15.out
01/01 00:01:57: job submitted
01/02 14:37:49: job started
01/02 17:52:49: job successfully completed

waited 17:50:52 


[C.1.q4.2] Sneaky job submission:

Reset the time to zero, to pretend the above never happened.
Inspect the state of the queue and answer the following questions:
How many nodes are currently in use by running jobs?
20 nodes are running jobs
How many nodes are currently idle?
13 nodes are idle
Submit a job to run myprogramsuccessfully, asking for as many nodes as possible so that your job can run right now (unless another competing job shows up in the nick of time!)

require 4 nodes

JOBNAME   USER       NODES  START TIME      REQ TIME   STATUS
job_0     euhlz      7      01/01 00:00:00  01:29:05   RUNNING
job_1     hoboha     13     01/01 00:00:00  04:55:05   RUNNING
job_15    slurm_user 4      01/01 00:02:55  08:00:05   RUNNING
job_2     hoboha     14     n/a             02:27:05   PENDING
job_3     pevux      21     n/a             04:32:05   PENDING
job_4     eay        26     n/a             08:46:05   PENDING
job_5     barm       14     n/a             08:14:05   PENDING
job_6     euhlz      26     n/a             04:09:05   PENDING

Inspect the state of the queue. Is your job running?
yes, my job was running right after I submitted
Advance time until your job completes.
Compare an contrast your job turnaround time with that in the previous question.

/$ cat job_15.out
01/01 00:02:55: job submitted
01/01 00:02:55: job started
01/01 07:02:55: job successfully completed

The job's turnaround time is 7 hours which is less than the previous turnaround time.


[C.1.q5.1] Asking for the right amount of time:
/$ cat job_4.out
01/01 00:01:21: job submitted
01/01 17:59:59: job started
01/01 21:14:59: job successfully completed

What is the job's turnaround time?
21:13:38
Could you have expected this behavior based on the state of the batch queue?
yes, because there were 2 jobs pendding and 2 jobs running ahead of my job.



01/01 00:00:47: job submitted
01/01 17:59:59: job started
01/01 21:14:59: job successfully completed

What is the job's turnaround time?
21:14:12
Could you have expected this behavior based on the state of the batch queue?
yes, because the require time are longer than the previous one, slurm must process it with lower priority.



[C.1.q6.1] Trying multiple job sizes:

Inspect the state of the queue. You should see that only 1 node is available right now.
If you were to submit a 1-node job right now, what turnaround time would you experience?
22 hours turnaround time

Submit a 2-node job, asking for just enough time for myprogramto complete successfully.
What is this job's turnaround time?
21:59:34
Reset the simulation and submit a 4-node job, asking for just enough time for myprogramto complete successfully.
When is this job's turnaround time?
17:29:33
Which option was better: using 1 node, 2 nodes, or 4 nodes?
4-nodes is better
Is there any way you could have predicted this based on initial state of the batch queue?
Feel free to experiment with different numbers of nodes, so see which one is best.
seems cannot find a best option. If require 4-nodes can run with job_2, so the waiting time is 8 hours less. Otherwise, need to wait until job_2 complete.