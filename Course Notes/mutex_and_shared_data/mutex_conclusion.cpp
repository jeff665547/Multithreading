// Mutex Conclusion
// * How to use the mutex effectivly
//   - Don't lock for any longer than necessary.
//     # Other threads waiting for the resource will have to wait longer. -> affect performance.
//   - Avoid input/output operations since these are very slow.
//   - Lock before accessing the resource, and unlock immediately before doing any further processing.
//     # Another private copy is useful.
//     # If we need to modify the resource, we can lock again. 
//       -> Give other threads the opportunities to run.
//   - Don't lock any more data than necessary.
//     # e.g. When accessing a single element in a linked list, don't lock the entire list
//            -> Blocking other threads which wants to access different elements.
//   - Don't make locking too fine-grained to the data. -> lead to a data race.
//     # Locking individual nodes for insertion and deletion operations separately causes a data race.
//       -> Other threads may interrupt and modify those surrounding nodes. -> data race.
//       -> Need to lock surrounding nodes in a single operation.
//   - When using a mutex to protect the shared data, use classes 
//     instead of global data (variables) and global functions.
//     # Make the shared data a member of the class.
//     # Make the mutex a member of the class.
//     # Make the task function a member function of the class.
//     # Provide a public getter function to let other callers (outside the calss) 
//       to retrieve the shared data (even when there are not any thread running).
//   - Don't use mutexes directly as possible.
// 
// * Pros and Cons of mutexes
//   Pros: 
//   - Prevent data races.
// 
//   Cons:
//   - Locking/Unlocking is a slow operation (since they need the help from the OS.)
//   - Low-level coding -- Height reliance on the programmer
//     1. Use the right mutex.
//     2. How different mutexes work.
//     3. How the different threads can modify the data.
//     -> At most time, don't use mutexes directly. Instead, use other higher level structures.
int main(){
    return 0;
}