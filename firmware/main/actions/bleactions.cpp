#include "bleactions.h"

// local includes
#include "ble.h"

void StartBleScanAction::triggered()
{
    ble::startScan();
}
