#include "mid_circ_buf.h"

/**************************************************************************
 * 私有辅助函数（嵌入式场景静态隐藏，减少开销）
 **************************************************************************/
// 校验缓冲区结构体有效性
static bool _CircBufOvSafe_Check_Valid(CircBufOverlaySafe_t* cb) {
    if (cb == NULL || cb->buf == NULL || cb->buf_size == 0 || cb->buf_size > CIRC_BUF_MAX_SIZE) {
        return false;
    }
    return true;
}

/**************************************************************************
 * 对外接口实现（安全覆盖模式，核心改进）
 **************************************************************************/
CircBufOverlaySafe_Status_t CircBufOvSafe_Init(CircBufOverlaySafe_t* cb, uint8_t* buf, uint32_t buf_size) {
    // 1. 参数校验（嵌入式场景严格防止无效配置）
    if (cb == NULL || buf == NULL) {
        return CIRC_BUF_OV_SAFE_ERR_NULL;
    }
    if (buf_size == 0 || buf_size > CIRC_BUF_MAX_SIZE) {
        return CIRC_BUF_OV_SAFE_ERR_INVALID_SIZE;
    }
    
    // 2. 初始化静态缓冲区参数（核心：设置is_empty=true，初始为空）
    memset(cb, 0, sizeof(CircBufOverlaySafe_t));
    cb->buf = buf;
    cb->buf_size = buf_size;
    cb->wr_idx = 0;
    cb->rd_idx = 0;
    cb->is_empty = true;  // 初始化空状态，消除后续歧义
    
    return CIRC_BUF_OV_SAFE_OK;
}
/* 
CircBufOverlaySafe_Status_t CircBufOvSafe_Write(CircBufOverlaySafe_t* cb, const uint8_t* data, uint32_t len) {
    // 1. 参数校验
    if (!_CircBufOvSafe_Check_Valid(cb) || data == NULL || len == 0) {
        return (len == 0) ? CIRC_BUF_OV_SAFE_OK : CIRC_BUF_OV_SAFE_ERR_NULL;
    }
    
    // 2. 覆盖模式核心：计算需要丢弃的旧数据长度，更新读指针（保证留存最新数据）
    uint32_t data_len_before = CircBufOvSafe_Get_Data_Len(cb);  // 写入前有效数据长度
    uint32_t data_len_after = data_len_before + len;            // 写入后理论数据长度
    uint32_t discard_len = 0;                                   // 需要丢弃的旧数据长度
    
    // 若写入后数据长度超过缓冲区容量，计算丢弃长度并更新读指针
    if (data_len_after > cb->buf_size) {
        discard_len = data_len_after - cb->buf_size;
        cb->rd_idx = (cb->rd_idx + discard_len) % cb->buf_size;  // 跳过被覆盖的旧数据
    }
    
    // 3. 跨区写入处理（分两次拷贝，自动跳转缓冲区首尾，保证数据完整性）
    uint32_t remaining = len;
    uint32_t current_wr = cb->wr_idx;  // 局部写指针，避免中间状态异常
    
    while (remaining > 0) {
        // 3.1 计算「当前写指针→缓冲区末尾」的可用字节数
        uint32_t len_to_end = cb->buf_size - current_wr;
        // 3.2 本次拷贝长度（不超过剩余数据，不超过到末尾的可用空间）
        uint32_t copy_len = (remaining < len_to_end) ? remaining : len_to_end;
        // 3.3 字节级拷贝（嵌入式高效，无需循环单个字节）
        memcpy(&cb->buf[current_wr], data + (len - remaining), copy_len);
        // 3.4 更新指针和剩余长度（取模实现环形跳转）
        current_wr = (current_wr + copy_len) % cb->buf_size; 
        remaining -= copy_len;
    }
    
    // 4. 更新全局写指针（批量写入完成后统一更新，保证安全）
    cb->wr_idx = current_wr;
    
    // 5. 核心改进：写入有效数据后，标记为非空（消除指针重合的空状态误判）
    cb->is_empty = false;
    
    return CIRC_BUF_OV_SAFE_OK;
}
 */
CircBufOverlaySafe_Status_t CircBufOvSafe_Write(CircBufOverlaySafe_t* cb, const uint8_t* data, uint32_t len) {
    // 1. 参数校验
    if (!_CircBufOvSafe_Check_Valid(cb) || data == NULL || len == 0) {
        return (len == 0) ? CIRC_BUF_OV_SAFE_OK : CIRC_BUF_OV_SAFE_ERR_NULL;
    }
    
    // 2. 初始化局部影子指针（避免中间状态污染全局变量，防脏读）
    uint32_t current_wr = cb->wr_idx;
    uint32_t current_rd = cb->rd_idx;
    const uint8_t* src_data = data;
    uint32_t write_len = len;

    // 3. 分场景精密处理：修复大块写入时的“二次覆盖”逻辑漏洞
    if (write_len >= cb->buf_size) {
        /* 情况 A：新写入的数据长度本身就超过或等于缓冲区总容量。
         * 此时旧数据注定被 100% 覆盖。缓冲区会被新数据的末尾部分填满。
         * 我们直接“剪枝”平移源数据指针，只保留最后能塞下的有效整屏数据。 */
        src_data = data + (write_len - cb->buf_size);
        write_len = cb->buf_size;
        
        /* 因为数据注定塞满，写入后读指针必须被强行同步到全新写指针的位置（两者重合） */
        current_rd = (current_wr + write_len) % cb->buf_size;
    } 
    else {
        /* 情况 B：新数据长度小于缓冲区总容量，属于正常的溢出覆盖 */
        uint32_t data_len_before = CircBufOvSafe_Get_Data_Len(cb);
        uint32_t data_len_after = data_len_before + write_len;
        
        if (data_len_after > cb->buf_size) {
            uint32_t discard_len = data_len_after - cb->buf_size;
            current_rd = (current_rd + discard_len) % cb->buf_size; // 仅跳过真正被挤掉的旧数据
        }
    }

    // 4. 顺序执行写入处理（彻底消除 while 循环，最多切分为两段拷贝）
    uint32_t len_to_end = cb->buf_size - current_wr; // 计算从当前写指针到缓冲区末尾的剩余空间

    if (write_len <= len_to_end) {
        // 子情况 1：剩余数据很短，直接一段 memcpy 就能写完，不需要折返
        memcpy(&cb->buf[current_wr], src_data, write_len);
        current_wr = (current_wr + write_len) % cb->buf_size; 
    } 
    else {
        // 子情况 2：数据长于到末尾的空间，必须一分为二，顺序折返拷贝
        uint32_t first_copy = len_to_end;               // 第一段：写满到物理末尾
        uint32_t second_copy = write_len - len_to_end;  // 第二段：折返回物理头部写剩下的

        // 第一段拷贝（到末尾）
        memcpy(&cb->buf[current_wr], src_data, first_copy);
        // 第二段拷贝（回头部）
        memcpy(&cb->buf[0], src_data + first_copy, second_copy);

        // 折返后，新的写指针位置其实就是第二段的长度
        current_wr = second_copy; 
    }

    // 5. 【一锤定音】所有数据在内存中彻底坐实后，再批量更新全局变量
    // 在汇编级别能最大化保证多任务、硬件中断环境下的时序安全
    cb->rd_idx = current_rd; // 瞬间更新全局读指针
    cb->wr_idx = current_wr; // 瞬间更新全局写指针
    cb->is_empty = false;    // 瞬间标记为非空

    return CIRC_BUF_OV_SAFE_OK;
}



/* CircBufOverlaySafe_Status_t CircBufOvSafe_Read_All(CircBufOverlaySafe_t* cb, uint8_t* data) {
    // 1. 参数校验（仅校验缓冲区有效性，移除len和actual_read相关判断，保持安全特性）
    if (!_CircBufOvSafe_Check_Valid(cb)) {
        return CIRC_BUF_OV_SAFE_ERR_NULL;
    }
    
    // 2. 依赖专用标志位判断是否为空，无歧义（保留原有核心逻辑，避免误判）
    if (cb->is_empty) {
        return CIRC_BUF_OV_SAFE_ERR_EMPTY;
    }
    
    // 3. 一次性获取全部有效数据长度（核心：读取所有有效数据，无需要求长度）
    uint32_t read = CircBufOvSafe_Get_Data_Len(cb);
    
    // 4. 跨区读取处理（分两次拷贝，自动跳转缓冲区首尾，保留跨区和数据完整性特性）
    uint32_t remaining = read;
    uint32_t current_rd = cb->rd_idx;  // 局部读指针，避免中间状态异常
    
    while (remaining > 0 && data != NULL) {
        // 4.1 计算「当前读指针→缓冲区末尾」的可用字节数
        uint32_t len_to_end = cb->buf_size - current_rd;
        // 4.2 本次拷贝长度（不超过剩余数据，不超过到末尾的可用空间）
        uint32_t copy_len = (remaining < len_to_end) ? remaining : len_to_end;
        // 4.3 字节级拷贝（保证数据顺序性和完整性，嵌入式高效拷贝）
        memcpy(data + (read - remaining), &cb->buf[current_rd], copy_len);
        // 4.4 更新指针和剩余长度（取模实现环形跳转，避免越界）
        current_rd = (current_rd + copy_len) % cb->buf_size;
        remaining -= copy_len;
    }
    
    // 5. 若data为NULL（丢弃全部数据），直接更新读指针到全部数据读取后的位置
    if (data == NULL) {
        current_rd = (current_rd + read) % cb->buf_size;
    }
    
    // 6. 更新全局读指针（批量读取完成后统一更新，保证安全）
    cb->rd_idx = current_rd;
    
    // 7. 更新is_empty标志位（读取全部数据后，缓冲区必然为空，直接置为true）
    // 无需计算剩余数据长度，简化逻辑（读取全部数据后remaining_data_len一定为0）
    cb->is_empty = true;
    
    // 8. 返回操作成功状态码
    return CIRC_BUF_OV_SAFE_OK;
} */


CircBufOverlaySafe_Status_t CircBufOvSafe_Read_All(CircBufOverlaySafe_t* cb, uint8_t* data) {
    // 1. 参数校验
    if (!_CircBufOvSafe_Check_Valid(cb)) {
        return CIRC_BUF_OV_SAFE_ERR_NULL;
    }
    
    // 2. 依赖专用标志位判断是否为空
    if (cb->is_empty) {
        return CIRC_BUF_OV_SAFE_ERR_EMPTY;
    }
    
    // 3. 一次性获取全部有效数据长度，并初始化影子读指针
    uint32_t read_len = CircBufOvSafe_Get_Data_Len(cb);
    uint32_t current_rd = cb->rd_idx;
    
    // 4. 顺序执行读取/丢弃处理（彻底干掉 while 循环，最多两段拷贝）
    uint32_t len_to_end = cb->buf_size - current_rd; // 当前读指针到缓冲区末尾的有效数据空间

    if (read_len <= len_to_end) {
        // 情况 A：所有有效数据连续存储，没有跨越缓冲区末尾
        if (data != NULL) {
            memcpy(data, &cb->buf[current_rd], read_len);
        }
        current_rd = (current_rd + read_ len) % cb->buf_size;
    } 
    else {
        // 情况 B：有效数据跨物理末尾折返，一分为二进行顺序拷贝
        uint32_t first_copy  = len_to_end;             // 第一段：从当前读指针到物理末尾
        uint32_t second_copy = read_len - len_to_end;  // 第二段：折返回物理头部的数据

        if (data != NULL) {
            // 第一段拷贝（从读指针到末尾）
            memcpy(data, &cb->buf[current_rd], first_copy);
            // 第二段拷贝（从物理头部开始）
            memcpy(data + first_copy, &cb->buf[0], second_copy);
        }
        
        // 折返后，读指针最终位置就是第二段数据的长度
        current_rd = second_copy;
    }
    
    // 5. 【一锤定音】数据安全搬运或丢弃完成后，批量原子更新全局状态
    cb->rd_idx = current_rd; // 瞬间更新全局读指针
    cb->is_empty = true;     // 既然是 Read_All，读完后缓冲区必然为空

    return CIRC_BUF_OV_SAFE_OK;
}
 CircBufOverlaySafe_Status_t CircBufOvSafe_Clear(CircBufOverlaySafe_t* cb) {
    // 1. 参数校验
    if (cb == NULL) {
        return CIRC_BUF_OV_SAFE_ERR_NULL;
    }
    
    // 2. 重置读写指针+恢复空状态（彻底清空，无歧义）
    cb->wr_idx = 0;
    cb->rd_idx = 0;
    cb->is_empty = true;
    
    return CIRC_BUF_OV_SAFE_OK;
}

uint32_t CircBufOvSafe_Get_Data_Len(CircBufOverlaySafe_t* cb) {
    if (!_CircBufOvSafe_Check_Valid(cb)) {
        return 0; // 既然是 uint32_t，返回 0 比返回 -1 更安全
    }
    
    // 1. 如果标记为空，有效长度绝对是 0
    if (cb->is_empty) {
        return 0;
    }
    
    // 2. 如果不为空，且指针重合，说明缓冲区是 100% 满状态
    if (cb->wr_idx == cb->rd_idx) {
        return cb->buf_size;
    }
    
    // 3. 正常非空、非满场景
    return (cb->wr_idx - cb->rd_idx + cb->buf_size) % cb->buf_size;
}

bool CircBufOvSafe_Is_Empty(CircBufOverlaySafe_t* cb) {
    if (!_CircBufOvSafe_Check_Valid(cb)) {
        return true;
    }
    
    // 核心改进：直接返回专用空状态标志位，彻底规避指针重合歧义
    return cb->is_empty;
}