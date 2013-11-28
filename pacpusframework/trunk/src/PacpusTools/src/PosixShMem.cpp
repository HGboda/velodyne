// %pacpus:license{
// This file is part of the PACPUS framework distributed under the
// CECILL-C License, Version 1.0.
/// @author  Gerald Dherbomez <firstname.surname@utc.fr>
/// @date    January, 2007
// %pacpus:license}

#include <cassert>
#include <cstdlib>
#include <iomanip>

#include <Pacpus/PacpusTools/PosixShMem.h>
#include <Pacpus/kernel/Log.h>
#include <Pacpus/kernel/cstdint.h>

DECLARE_STATIC_LOGGER("pacpus.core.PosixShMem");

using std::hex;

//////////////////////////////////////////////////////////////////////////
/// Constructor
PosixShMem::PosixShMem(const char * name, int size)
{
    LOG_TRACE("constructor(" << name << ", " << size << ")");
    LOG_INFO("creating shared memory" << "\t"
             << "name=" << name << "\t"
             << "size=" << size
             );

    {
        QString memoryName = QString(name);
        memory_ = new QSharedMemory(memoryName);
    }
    if (!memory_) {
        LOG_FATAL("out of memory: cannot create shared memory object");
        exit(-1);
    }

    bool result = false;
    LOG_DEBUG("trying to create a memory segment");
    if (!memory_->create(size)) {
        if (QSharedMemory::AlreadyExists == memory_->error()) {
            LOG_DEBUG("trying to attach to an existing memory segment");
            if (!memory_->attach()) {
                LOG_ERROR("cannot attach to an existing shared memory segment '" << name << "'"
                          << ". error code: " << memory_->error()
                          << ". error message:" << memory_->errorString().toStdString()
                          );
            } else {
                LOG_INFO("attached shared memory segment '" << name << "'");
                int attachedMemorySize = memory_->size();
                LOG_INFO("attached memory size = " << attachedMemorySize);
                if (attachedMemorySize < size) {
                    LOG_ERROR("attached memory is smaller than requested, unexpected behaviour possible");
                    LOG_INFO("use system tools like 'ipcs' and 'ipcrm' to remove shared memory and re-run");
                }
                result = true;
            }
        } else {
            LOG_ERROR("cannot create shared memory segment '" << name << "'"
                      << ". error code: " << memory_->error()
                      << ". error message:" << memory_->errorString().toStdString()
                      );
        }
    } else {
        LOG_INFO("created shared memory segment '" << name << "'");
        result = true;
    }
    
    /////////////////////////////////////////
    if (result) {
        // create the equivalent Windows event (autoreset mode?)
        QString eventName = "EvtShMem_" + QString(name);
        LOG_DEBUG("creating event '" << eventName << "'...");
        event_ = new QSystemSemaphore(eventName, 0, QSystemSemaphore::Create);
        if (event_) {
            LOG_DEBUG("created event '" << eventName << "'");
        } else {
            LOG_ERROR("cannot create event '" << eventName << "'"
                      );
        }

        // get a pointer to the shared memory segment
        shMem_ = memory_->data();
    } else {
        LOG_FATAL("cannot create shared memory segment '" << name << "'");
        //throw MemoryCreationError();
        exit(memory_->error());
    }
}

//////////////////////////////////////////////////////////////////////////
/// Destructor
PosixShMem::~PosixShMem()
{
    LOG_TRACE("destructor");

    // detach this process from the shared memory
    memory_->detach();
    // free shared memory
    delete memory_;
    // free event
    delete event_;
}

//////////////////////////////////////////////////////////////////////////
/// Use this method to get the data of the shared memory
void * PosixShMem::read()
{
    return shMem_;
}

//////////////////////////////////////////////////////////////////////////
/// Use this method to write data in shared memory. Offset is given in bytes
void PosixShMem::write(void * data, int size, unsigned long offset)
{
    LOG_TRACE("writing " << size << " bytes to shared memory");

    lockMemory();
    uint32_t * dest = (uint32_t *) shMem_ + offset;

    LOG_TRACE("adresses:"
              << " shm = " << hex << shMem_
              << " dst = " << hex << dest
              << " src = " << hex << data
              );

    memcpy(dest, data, size);
    unlockMemory();
    event_->release();
}

//////////////////////////////////////////////////////////////////////////
/// Function that locks access to the shared memory
void PosixShMem::lockMemory()
{
    assert(memory_);
    if (!memory_->lock()) {
        LOG_ERROR("cannot lock memory '" << memory_->key().toStdString() << "'");
    }
}

//////////////////////////////////////////////////////////////////////////
/// Function that unlocks access to the shared memory
void PosixShMem::unlockMemory()
{
    assert(memory_);
    if (!memory_->unlock()) {
        LOG_ERROR("cannot unlock memory '" << memory_->key().toStdString() << "'");
    }
}

//////////////////////////////////////////////////////////////////////////
/// Use this method to wait the incoming of new data
/// you can specify a timeout in ms to avoid infinite blocking or 0 (infinite)
/// return true if new data available before the timeout or else false
bool PosixShMem::wait(unsigned long /*timeout*/)
{
    bool result = event_->acquire();
    LOG_TRACE("event acquired: note that timeout doesn't work yet on linux platform");
    return result;
}