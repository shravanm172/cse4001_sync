Problem 1: No-starve reader-writer solution
<img width="2559" height="1439" alt="image" src="https://github.com/user-attachments/assets/b3028c25-0834-4df6-8c9a-acc6ce1cb695" />
This solution uses a turnstile and a reader lightswitch to guarantee that neither readers nor writers starve. The turnstile forces both readers and writers to line up in arrival order, while the lightswitch lets multiple readers enter together without blocking each other. Writers wait only when readers are inside, and once a writer arrives, new readers are briefly held at the turnstile. This ensures fair access to the shared resource with no starvation.

Problem 2: Writer-priority Readers–Writers
<img width="2556" height="1434" alt="image" src="https://github.com/user-attachments/assets/d80ed30c-0e96-42d2-9b0b-f22191698f8d" />
This solution gives priority to writers by blocking new readers whenever a writer arrives. A writer lightswitch locks the noReaders semaphore so that subsequent readers queue, while a reader lightswitch manages access to noWriters so readers inside can finish. Writers then proceed one-by-one without being overtaken by incoming readers. This enforces a writer-first policy where readers may wait, but writers never starve.

Problem 3: Dining Philosophers Solution #1
<img width="2559" height="1439" alt="image" src="https://github.com/user-attachments/assets/94f2536c-2e0f-4f65-a0d5-7be8d6ff9b95" />
To prevent deadlock, this solution limits the number of philosophers who may attempt to eat at the same time. A “footman” (multiplex) semaphore allows only four philosophers to pick up forks, ensuring that not all five can be stuck holding one fork and waiting for the other. Each philosopher must acquire permission from the footman before picking up forks, which guarantees that at least one philosopher can always obtain both forks, eat, and release them.

Problem 4: Dining Philosophers Solution #2
<img width="2559" height="1439" alt="image" src="https://github.com/user-attachments/assets/8ee5b7ed-1e4d-425c-b0c9-0d59fb48c7ed" />
This solution removes deadlock by making philosophers asymmetric: even-numbered philosophers pick up the right fork first, while odd-numbered philosophers pick up the left fork first. This ensures that not all philosophers compete for the same fork order, breaking the circular wait condition. Because at least one philosopher is always able to pick up both forks, deadlock cannot occur, and philosophers continue to alternate between thinking and eating.
