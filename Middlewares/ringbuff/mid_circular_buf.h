#ifndef MID_CIRCULAR_BUF_H
#define MID_CIRCULAR_BUF_H


/**************************************************************************
 * 头文件包含（仅依赖嵌入式C标准库）
 **************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/**************************************************************************
 * 配置项（根据实际场景修改）
 **************************************************************************/
#ifndef CIRC_BUF_MAX_SIZE
#define CIRC_BUF_MAX_SIZE    1024U  // 缓冲区最大容量限制，防止内存耗尽
#endif

/**************************************************************************
 * 环形缓冲区结构体（新增is_empty标志位，消除歧义）
 **************************************************************************/
typedef struct {
    uint8_t*     buf;        // 静态缓冲区首地址（用户提供，指定SRAM/DTCM等）
    uint32_t     buf_size;   // 缓冲区总容量（字节数，全部可用，无预留）
    uint32_t     wr_idx;     // 写指针：下一个待写入的字节索引
    uint32_t     rd_idx;     // 读指针：下一个待读取的字节索引
    bool         is_empty;   // 专用空状态标志位（核心改进：true=空，false=非空）
} CircBufOverlaySafe_t;

/**************************************************************************
 * 操作状态码（简化，仅保留核心状态）
 **************************************************************************/
typedef enum {
    CIRC_BUF_OV_SAFE_OK = 0,          // 操作成功
    CIRC_BUF_OV_SAFE_ERR_NULL,        // 空指针错误
    CIRC_BUF_OV_SAFE_ERR_EMPTY,       // 缓冲区空，无法读取
    CIRC_BUF_OV_SAFE_ERR_INVALID_SIZE // 无效缓冲区大小（≤0或超过最大限制）
} CircBufOverlaySafe_Status_t;

/**************************************************************************
 * 对外核心接口（安全覆盖模式，嵌入式友好）
 **************************************************************************/
/**
 * @brief  环形缓冲区初始化（纯静态，绑定用户提供的静态内存）
 * @param  cb  环形缓冲区指针
 * @param  buf 外部提供的静态缓冲区首地址（如：uint8_t buf[128];）
 * @param  buf_size  缓冲区总容量（字节数，≥1即可，全部可用）
 * @retval CircBufOverlaySafe_Status_t  操作状态码
 */
CircBufOverlaySafe_Status_t CircBufOvSafe_Init(CircBufOverlaySafe_t* cb, uint8_t* buf, uint32_t buf_size);

/**
 * @brief  环形缓冲区写入数据（覆盖模式，支持跨区，满则自动覆盖旧数据）
 * @param  cb  环形缓冲区指针
 * @param  data  待写入的数据指针（连续字节数组）
 * @param  len  待写入的字节长度
 * @retval CircBufOverlaySafe_Status_t  操作状态码
 */
CircBufOverlaySafe_Status_t CircBufOvSafe_Write(CircBufOverlaySafe_t* cb, const uint8_t* data, uint32_t len);


CircBufOverlaySafe_Status_t CircBufOvSafe_Read_All(CircBufOverlaySafe_t* cb, uint8_t* data);
/**
 * @brief  清空环形缓冲区（重置读写指针+恢复空状态）
 * @param  cb  环形缓冲区指针
 * @retval CircBufOverlaySafe_Status_t  操作状态码
 */
CircBufOverlaySafe_Status_t CircBufOvSafe_Clear(CircBufOverlaySafe_t* cb);

/**
 * @brief  查询环形缓冲区当前有效数据长度（字节数）
 * @param  cb  环形缓冲区指针
 * @retval uint32_t  有效数据长度（0表示空，≤buf_size）
 */
uint32_t CircBufOvSafe_Get_Data_Len(CircBufOverlaySafe_t* cb);

/**
 * @brief  判断环形缓冲区是否为空（依赖专用标志位，无歧义）
 * @param  cb  环形缓冲区指针
 * @retval bool  true=空，false=非空
 */
bool CircBufOvSafe_Is_Empty(CircBufOverlaySafe_t* cb);

#endif /* CIRCULAR_BUF_OVERLAY_SAFE_H */