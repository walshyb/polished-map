import React, { useEffect } from 'react';
import { MenuItem, Menu, Dropdown } from 'semantic-ui-react'
import { useState } from 'react';
import { useAppSelector, useAppDispatch } from '../../hooks';
import { processFile } from '../../store/fileSlice';

interface HTMLInputEvent extends Event {
  target: HTMLInputElement & EventTarget;
}
export default function TopBar(
  { fileProcessed, setFileProcessed }: { fileProcessed: boolean; setFileProcessed: (fileProcessed: boolean) => void }
) {
  const [activeItem, setActiveItem] = useState<string>('');
  const dispatch = useAppDispatch();
  const status = useAppSelector((state) => state.file.state);

  
  const handleFileUpload = async (event: React.ChangeEvent<HTMLInputElement>) => {
    const file: any = event?.target.files && event.target.files[0];
    
    if (file) {
      setFileProcessed(false);
      // Convert file to ArrayBuffer
      const arrayBuffer: ArrayBuffer = await file.arrayBuffer();

      // Pass arrayBuffer to WebAssembly module
      dispatch(
        processFile({
          arrayBuffer,
          size: arrayBuffer.byteLength,
          filename: file.name
        })
      );
    }
  };

  return (
      <Menu
        inverted={true}
      >
        <Menu.Item>
          Logo Logo
        </Menu.Item>
        <Dropdown item text='File'>
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
