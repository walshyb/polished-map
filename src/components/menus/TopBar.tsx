import { processFile } from '../../utils/wasm-funcs';
import React, { Component } from 'react'
import { MenuItem, Menu, Dropdown } from 'semantic-ui-react'
import { useState } from 'react';


interface HTMLInputEvent extends Event {
  target: HTMLInputElement & EventTarget;
}
export default function TopBar() {
  const [activeItem, setActiveItem] = useState<string>('');
  
  const handleFileUpload = async (event: React.ChangeEvent<HTMLInputElement>) => {
    const file: any = event?.target.files && event.target.files[0];
    
    if (file) {
      // Convert file to ArrayBuffer
      const arrayBuffer: ArrayBuffer = await file.arrayBuffer();

      // Pass arrayBuffer to WebAssembly module
      processFile(arrayBuffer, arrayBuffer.byteLength, file.name);
    }
  };

  return (
      <Menu>
        <Dropdown open={true} item text='File'>
          <Dropdown.Menu>
            <Dropdown.Item>New</Dropdown.Item>
            <Dropdown.Item>
              <label htmlFor='file-upload' style={{ cursor: 'pointer' }}>
                Open...
                <input
                  id='file-upload'
                  type='file'
                  style={{ display: 'none' }}
                  onChange={handleFileUpload}
                />
              </label>
            </Dropdown.Item>
          </Dropdown.Menu>
        </Dropdown>

        <MenuItem
          name='data'
          active={activeItem === 'data'}
          onClick={() => setActiveItem('data')}
        >
          Data
        </MenuItem>

        <MenuItem
          name='Edit'
          active={activeItem === 'edit'}
          onClick={() => setActiveItem('edit')}
        >
          Edit
        </MenuItem>
      </Menu>
    )
}
