#import <mach/host_info.h>
#import <mach/mach_host.h>

#include <iostream>

using std::cout;
using std::endl;

#define moniker(x) ( (x) >= 1024 ? ( (x) < 1048576 ? 'M' : 'G' ) : 'K' )
#define truesize(x) ( (x) >= 1024 ? ( (x) < 1048576 ? (x)/1024 : (x)/1048576 ) : (x) )

vm_statistics_data_t page_info;
vm_size_t pagesize;
mach_msg_type_number_t count;
kern_return_t kret;

int main(int argc, const char **argv)
{
    pagesize = 0;
    kret = host_page_size (mach_host_self(), &pagesize);

    // vm stats
    count = HOST_VM_INFO_COUNT;
    kret = host_statistics (mach_host_self(), HOST_VM_INFO,
                            (host_info_t)&page_info, &count);

    if (kret == KERN_SUCCESS)
    {
        unsigned int pw, pa, pi, pf;
        float pu;

        // Calc kilobytes
        pw = page_info.wire_count*pagesize / 1024;
        pa = page_info.active_count*pagesize / 1024;
        pi = page_info.inactive_count*pagesize / 1024;
        pf = page_info.free_count*pagesize / 1024;

        pu = pw+pa+pi;

        cout << "pu = " << pu << " pw = " << pw << " pa = " << pa << " pi = " << pi << endl;

        cout << "free = " << pf << endl;

    }

    return 0;
}
