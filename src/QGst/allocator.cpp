/*
    Copyright (C) 2014  Diane Trout <diane@ghic.org>

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "allocator.h"
#include <gst/gstobject.h>
#include <gst/gstminiobject.h>
#include <gst/gstallocator.h>

namespace QGst {

struct AllocationParamsPrivate : GstAllocationParams
{
};

AllocationParams::AllocationParams()
  : d_ptr(new AllocationParamsPrivate)
{
    Q_D(AllocationParams);
    gst_allocation_params_init(d);
}

AllocationParams::AllocationParams(AllocationParams &other)
  : d_ptr(static_cast<AllocationParamsPrivate *>(gst_allocation_params_copy(other.d_ptr)))
{
}

AllocationParams::~AllocationParams()
{
    gst_allocation_params_free(d_ptr);
}

MemoryFlags AllocationParams::flags() const
{
    Q_D(const AllocationParams);
    return static_cast<QGst::MemoryFlags>(static_cast<unsigned int>(d->flags));
}

void AllocationParams::setFlags(MemoryFlags flags)
{
    Q_D(AllocationParams);
    d->flags = static_cast<GstMemoryFlags>(static_cast<unsigned int>(flags));
}

size_t AllocationParams::align() const
{
    Q_D(const AllocationParams);
    return d->align;
}

void AllocationParams::setAlign(size_t align)
{
    Q_D(AllocationParams);
    d->align = align;
}

size_t AllocationParams::prefix() const
{
    Q_D(const AllocationParams);
    return d->prefix;
}

void AllocationParams::setPrefix(size_t align)
{
    Q_D(AllocationParams);
    d->prefix = align;
}

size_t AllocationParams::padding() const
{
    Q_D(const AllocationParams);
    return d->padding;
}

void AllocationParams::setPadding(size_t padding)
{
    Q_D(AllocationParams);
    d->padding = padding;
}

//static
AllocatorPtr Allocator::find(const char *name)
{
    return AllocatorPtr::wrap(gst_allocator_find(name), false);
}

//static
AllocatorPtr Allocator::getDefault()
{
    return find(NULL);
}

//static
AllocatorPtr Allocator::getSystemMemory()
{
    return find(GST_ALLOCATOR_SYSMEM);
}

MemoryPtr Allocator::alloc(size_t size, const AllocationParams & params)
{
    return MemoryPtr::wrap(gst_allocator_alloc(object<GstAllocator>(), size,
            const_cast<GstAllocationParams *>(static_cast<const GstAllocationParams *>(params.d_ptr))), false);
}

void Allocator::free(MemoryPtr & memory)
{
    GstMemory *mem = memory;
    gst_memory_ref(mem);
    memory.clear();
    gst_allocator_free(object<GstAllocator>(), mem);
}

} /* QGst */
